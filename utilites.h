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
		virtual_maneuver_circle_to_tangent_elleptic_raise = 1, // касающиеся орбиты
		virtual_maneuver_circle_to_tangent_elleptic_descend = 2,// касающиеся орбиты

		virtual_maneuver_elleptic_to_tangent_circle_raise = 3, // касающиеся орбиты
		virtual_maneuver_elleptic_to_tangent_circle_descend = 4, // касающиеся орбиты

		virtual_maneuver_circle_to_circle_raise_hohmann = 5,// с круговой на круговую
		virtual_maneuver_circle_to_circle_descend_hohmann = 6,// с круговой на круговую
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
	 * @brief структура описывает мгновенные импульсные маневры
	 */
	struct maneuver_scheme
	{
		const kepler_orbit init_orbit;
		const kepler_orbit target_orbit;
		std::vector<virtual_transfer_orbit> transfer_orbits;
		virtual_maneuver_type type;

		maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit):init_orbit(_init_orbit), target_orbit(_target_orbit)
		{
			if( init_orbit.eccentricity == 0 && init_orbit.pericenter ==  target_orbit.pericenter && init_orbit.apocenter < target_orbit.apocenter)
			{
				from_circular_to_elleptic_r(init_orbit, target_orbit, transfer_orbits);
				type = virtual_maneuver_circle_to_tangent_elleptic_raise;
			}

			else if( init_orbit.eccentricity == 0 && init_orbit.apocenter ==  target_orbit.apocenter && init_orbit.pericenter > target_orbit.pericenter)
			{
				from_circular_to_elleptic_d(init_orbit, target_orbit, transfer_orbits);
				type = virtual_maneuver_circle_to_tangent_elleptic_descend;
			}

			else if(init_orbit.eccentricity == 0 &&  target_orbit.eccentricity == 0 && init_orbit.pericenter < target_orbit.pericenter)
			{
				from_circle_to_circle_raise_hohmann(_init_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_circle_to_circle_raise_hohmann;
			}

			else if(init_orbit.eccentricity == 0 &&  target_orbit.eccentricity == 0 && init_orbit.pericenter > target_orbit.pericenter)
			{
				from_circle_to_circle_descend_hohmann(_init_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_circle_to_circle_descend_hohmann;
			}
		}

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

		/// с круговой на внешнюю эллиптическую орбиту
		void from_circular_to_elleptic_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
		{
			assert(_init_orbit.eccentricity == 0);
			assert(_init_orbit.pericenter ==  _target_orbit.pericenter);
			assert(_init_orbit.apocenter < _target_orbit.apocenter);
			assert(_target_orbit.pericenter_angle >= 0 );

			double vc = circle_velocity(_init_orbit); 
			double vp = pericenter_velocity(_target_orbit);

			double impulse = vp - vc;

			
			virtual_transfer_orbit vto;
			vto.init_orbit = _init_orbit;
			vto.finish_orbit = _target_orbit;
			vto.impulse = impulse;
			vto.type = virtual_maneuver_circle_to_tangent_elleptic_raise;
			vto.imp_true_anomaly = _target_orbit.pericenter_angle;

			_transfer_orbits.push_back(vto);
		}

		/// с круговой на внутренюю эллиптическую орбиту
		void from_circular_to_elleptic_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
		{
			assert(_init_orbit.eccentricity == 0);
			assert(_init_orbit.apocenter == _target_orbit.apocenter);
			assert(_init_orbit.pericenter > _target_orbit.pericenter);
			assert(_target_orbit.pericenter_angle >= 0 );

			double vc = circle_velocity(_init_orbit); 
			double va = apocenter_velocity(_target_orbit);

			double impulse = vc - va;

			virtual_transfer_orbit vto;
			vto.init_orbit = init_orbit;
			vto.finish_orbit = _target_orbit;
			vto.impulse = (-1)*impulse; // для понижения орбиты импульс направлен в сторону, противоположную движению
			vto.imp_true_anomaly = (_target_orbit.pericenter_angle < PiConst) ? _target_orbit.pericenter_angle + PiConst : _target_orbit.pericenter_angle - PiConst;
			vto.type = virtual_maneuver_circle_to_tangent_elleptic_descend;

			transfer_orbits.push_back(vto);
		}

		void from_elleptic_to_circular_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
		{
			assert(_init_orbit.eccentricity != 0);
			assert(_init_orbit.apocenter == _target_orbit.apocenter);
			assert(_init_orbit.pericenter < _target_orbit.pericenter);
			assert(_target_orbit.pericenter_angle >= 0 );

			double va = apocenter_velocity(_init_orbit);
			double vc = circle_velocity(_target_orbit); 

			double impulse = vc - va;

			virtual_transfer_orbit vto;
			vto.init_orbit = init_orbit;
			vto.finish_orbit = _target_orbit;
			vto.impulse = impulse;
			vto.type = virtual_maneuver_elleptic_to_tangent_circle_raise;
			vto.imp_true_anomaly = PiConst; // прикладываем импульс в апоцентре

			_transfer_orbits.push_back(vto);
		}

		void from_elleptic_to_circular_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
		{
			assert(_init_orbit.eccentricity != 0);
			assert(_init_orbit.pericenter == _target_orbit.pericenter);
			assert(_init_orbit.apocenter > _target_orbit.apocenter);
			assert(_target_orbit.pericenter_angle >= 0 );

			double vp = pericenter_velocity(_init_orbit);
			double vc = circle_velocity(_target_orbit); 

			double impulse = vc - vp;

			virtual_transfer_orbit vto;
			vto.init_orbit = _init_orbit;
			vto.finish_orbit = _target_orbit;
			vto.impulse = impulse;
			vto.type = virtual_maneuver_elleptic_to_tangent_circle_descend;
			vto.imp_true_anomaly = 0; // прикладываем импульс в перицентре

			_transfer_orbits.push_back(vto);
		}

		void from_circle_to_circle_raise_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
		{
			assert(_init_orbit.eccentricity == 0 && _target_orbit.eccentricity == 0);
			assert(_init_orbit.apocenter < _target_orbit.apocenter && _init_orbit.pericenter < _target_orbit.pericenter);
			assert(_target_orbit.pericenter_angle >= 0 );

			kepler_orbit elleptic_transfer;// эллипс Гомана

			elleptic_transfer.pericenter = _init_orbit.pericenter;
			elleptic_transfer.apocenter = _target_orbit.apocenter;
			elleptic_transfer.eccentricity = calc_eccentricity(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
			elleptic_transfer.focal = calc_focal(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
			elleptic_transfer.pericenter_angle = 0;
			
			from_circular_to_elleptic_r(_init_orbit, elleptic_transfer, _transfer_orbits);
			assert(_transfer_orbits.size());
			from_elleptic_to_circular_r(elleptic_transfer, _target_orbit, _transfer_orbits);

		}

		void from_circle_to_circle_descend_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
		{
			assert(_init_orbit.eccentricity == 0 && _target_orbit.eccentricity == 0);
			assert(_init_orbit.apocenter > _target_orbit.apocenter && _init_orbit.pericenter > _target_orbit.pericenter);
			assert(_target_orbit.pericenter_angle >= 0 );

			kepler_orbit elleptic_transfer;// эллипс Гомана

			elleptic_transfer.pericenter = _target_orbit.pericenter;
			elleptic_transfer.apocenter = _init_orbit.apocenter;
			elleptic_transfer.eccentricity = calc_eccentricity(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
			elleptic_transfer.focal = calc_focal(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
			elleptic_transfer.pericenter_angle = 0;
			
			from_circular_to_elleptic_d(_init_orbit, elleptic_transfer, _transfer_orbits);
			assert(_transfer_orbits.size());
			from_elleptic_to_circular_d(elleptic_transfer, _target_orbit, _transfer_orbits);

		}


	};

}


#endif // !UTILITES_H
