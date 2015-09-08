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


	double calc_focal(double pericenter, double apocenter);
}

namespace utilites
{
	enum virtual_maneuver_type
	{
		virtual_maneuver_unknown = 0,
		virtual_maneuver_circle_to_elleptic_ep_ha = 1, // ep is equal pericenter, ha is higher apocenter
	};

	struct virtual_transfer_orbit
	{
		kepler_orbit orbit;
		virtual_maneuver_type type;
	};


	/**
	 * @brief структура описывает мгновенные импульсные маневры
	 */
	struct virtual_maneuver
	{
		std::vector<double> impulses; // мс
		std::vector<virtual_transfer_orbit> transfer_orbits;
		virtual_maneuver_type type;

		/*virtual_maneuver(kepler_orbit & init_orbit, kepler_orbit & target_orbit)
		{
			if( init_orbit.eccentricity == 0 && init_orbit.pericenter ==  target_orbit.pericenter && init_orbit.apocenter < target_orbit.apocenter)
			{
				double vc = circle_velocity(init_orbit); 
				double vp = pericenter_velocity(target_orbit);

				double impulse = vp - vc;
				impulses.push_back(impulse);

				kepler_orbit transfer;
				transfer.apocenter = target_orbit.apocenter;
				transfer.pericenter = init_orbit.pericenter;
				transfer.eccentricity = calc_eccentricity(init_orbit.pericenter, target_orbit.apocenter);
				transfer.focal =  calc_focal(init_orbit.pericenter, target_orbit.apocenter);
				transfer.pericenter_angle = init_orbit.pericenter_angle;

				virtual_transfer_orbit vto;
				vto.orbit = transfer;
				vto.type = virtual_maneuver_circle_to_elleptic_ep_ha;

				transfer_orbits.push_back(vto);
			}
			else if( init_orbit.eccentricity == 0 && init_orbit.apocenter ==  target_orbit.apocenter && init_orbit.pericenter > target_orbit.pericenter)
			{
				double vc = circle_velocity(init_orbit); 
				double va = apocenter_velocity(target_orbit);

				double impulse = va - vc;
				impulses.push_back(impulse);

				kepler_orbit transfer;
				transfer.apocenter = init_orbit.apocenter;
				transfer.pericenter = target_orbit.pericenter;
				transfer.eccentricity = calc_eccentricity(target_orbit.pericenter, init_orbit.apocenter);
				transfer.focal =  calc_focal(target_orbit.pericenter, init_orbit.apocenter);
				transfer.pericenter_angle = init_orbit.pericenter_angle;

				virtual_transfer_orbit vto;
				vto.orbit = transfer;
				vto.type = virtual_maneuver_circle_to_elleptic_ep_ha;

				transfer_orbits.push_back(vto);
			}
		}*/

	private:

		double circle_velocity(const kepler_orbit& orbit)
		{
			assert(orbit.apocenter == orbit.pericenter);
			assert(orbit.apocenter!=0);

			double result = sqrt ( EarthGravy / orbit.apocenter );
			return result;
		}

		double pericenter_velocity(const kepler_orbit& orbit)
		{
			assert(orbit.apocenter);
			assert(orbit.pericenter);
			assert(orbit.apocenter >= orbit.pericenter);

			double result = sqrt ( 2 * EarthGravy /( orbit.pericenter + orbit.apocenter) * orbit.apocenter / orbit.pericenter );
			return result ;
		}

		double apocenter_velocity(const kepler_orbit& orbit)
		{
			assert(orbit.apocenter);
			assert(orbit.pericenter);
			assert(orbit.apocenter >= orbit.pericenter);

			double result = sqrt ( 2 * EarthGravy /( orbit.pericenter + orbit.apocenter) * orbit.pericenter / orbit.apocenter );
			return result ;
		}

	};

}


#endif // !UTILITES_H
