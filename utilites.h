#ifndef UTILITES_H
#define UTILITES_H
#include "constants.h"
#include <assert.h>
#include "orbit.h"
namespace utilites
{
	const size_t MAX_ITERATIONS = 100;
	/**
	 * @brief ConvertMeanToEccentric
	 * преобразование средней аномалии в эксцентрическую
	 * источник http://murison.alpheratz.net/dynamics/twobody/KeplerIterations_summary.pdf
	 * версия на python ( и автор реализации) https://github.com/RazerM/orbital
	 */
	double ConvertEccentricFromMean(double mean_anomaly, double eccentricity, double tolerance);

	double ConvertEccentricFromTrue(double true_anomaly, double eccentricity);

	double ConvertMeanFromEccentric(double eccentric_anomaly, double eccentricity);

	double ConvertMeanFromTrue(double true_anomaly, double eccentricity);

	double ConvertTrueFromEccentric(double eccentric_anomaly, double eccentricity);

	double ConvertTrueFromMean(double mean_anomaly, double eccentricity);


	/** семейство функций dif_ - оскулирующие элементы орбиты */
	double dif_ecc(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_focal(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_periapsis_arg(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_periapsis(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_apsis(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_tetta(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	/** проекции ускарения */
	double dif_ecc_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_focal_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_pericenter_ang_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_tetta_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_sigma_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	/**
	 * @brief приращение скорости для изменения высоты апоцентра
	 * прикладывается в перицентре
	 */
	double velocity_raise_apo(kepler_orbit* current_orbit, double new_apocenter, const double gravy = EarthGravy); /// м/с

	/**
	 * @brief приращение скорости для изменения высоты перицентра
	 * прикладывается в апоцентре
	 */
	double velocity_raise_peri(kepler_orbit* current_orbit, double new_pericenter, const double gravy = EarthGravy);

	typedef double(*Elements_f)(double, double, double, CEllepticalOrbit *);

	void DebugPrintOrbit( CEllepticalOrbit&);

	double calc_eccentricity (double pericenter, double apocenter);

	double calc_eccentricity (const kepler_orbit & orbit); 

	double calc_focal(double pericenter, double apocenter);

	double calc_focal(const kepler_orbit & orbit);

	kepler_orbit make_circle(double radius);
}

namespace utilites
{
	enum virtual_maneuver_type
	{
		virtual_maneuver_unknown = 0,
		virtual_maneuver_circle_to_tangent_elleptic_raise = 1, // компланарный, касающиеся орбиты
		virtual_maneuver_circle_to_tangent_elleptic_descend = 2,// компланарный, касающиеся орбиты

		virtual_maneuver_elleptic_to_tangent_circle_raise = 3, // компланарный, касающиеся орбиты
		virtual_maneuver_elleptic_to_tangent_circle_descend = 4, // компланарный, касающиеся орбиты

		virtual_maneuver_circle_to_circle_raise_hohmann = 5,// компланарный, с круговой на круговую
		virtual_maneuver_circle_to_circle_descend_hohmann = 6,// компланарный, с круговой на круговую

		virtual_maneuver_elleptic_apocenter_raise = 7, // компланарный, поднять апоцентр
		virtual_maneuver_elleptic_apocenter_descend = 8, // компланарный, поднять апоцентр

		virtual_maneuver_elleptic_pericenter_raise = 9, // компланарный, поднять перицентр
		virtual_maneuver_elleptic_pericenter_descend = 10, // компланарный, поднять перицентр

		virtual_maneuver_elleptic_to_cross_elleptic = 11, // компланарный, пересекающиеся орбиты
	};

	struct virtual_transfer_orbit
	{
		kepler_orbit init_orbit; // орбита, на которой прикладывается импульс
		kepler_orbit finish_orbit; //орбита, после приложения импульса
		double impulse;
		double imp_true_anomaly; // точка приложения импульса, на начальной орбите
		virtual_maneuver_type type;
	};


	/**
	 * @brief структура описывает мгновенные импульсные плоские маневры
	 */
	struct maneuver_scheme
	{
		const kepler_orbit init_orbit;
		const kepler_orbit target_orbit;
		std::vector<virtual_transfer_orbit> transfer_orbits;
		virtual_maneuver_type type;

		maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit);

	private:
		static double circle_velocity(const kepler_orbit& orbit);

		static double pericenter_velocity(const kepler_orbit& orbit);

		static double apocenter_velocity(const kepler_orbit& orbit);

		/// с круговой на внешнюю эллиптическую орбиту
		static void from_circle_to_elleptic_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		/// с круговой на внутренюю эллиптическую орбиту
		static void from_circle_to_elleptic_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void from_elleptic_to_circle_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void from_elleptic_to_circle_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void from_circle_to_circle_raise_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void from_circle_to_circle_descend_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void from_elleptic_apocenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void from_elleptic_pericenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		//с эллептической на эллиптическую по окружности
		void from_elleptic_to_elleptic_circle_transfer(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
	public:
		static void change_apocenter(const kepler_orbit & _init_orbit, double new_apocenter, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void change_pericenter(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		static void change_pericenter_argument(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
	};

}


#endif // !UTILITES_H
