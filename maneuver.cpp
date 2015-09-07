#include "maneuver.h"
#include "utilites.h"

using namespace utilites;

using namespace maneuver;

namespace maneuver
{
	///реализация конструктора пассивного участка (maneuver.h)
	CPassivePath::CPassivePath(CEllepticalOrbit _orbit,  double _begin_true_anomaly,
			double _end_true_anomaly, double _step): orbit(_orbit),
		begin_true_anomaly(_begin_true_anomaly),
		end_true_anomaly(_end_true_anomaly),
		step(_step)
	{
		duration = 0;

		// Получаем кеплеровы элементы орбиты
		kepler_orbit elements = orbit.GetKeplerOrbitFormat();

		// начальная эксцентрическая анномалия
		double begin_ecc = ConvertEccentricFromTrue(begin_true_anomaly, elements.eccentricity);
		
		// конечная эксцентрическая аномалия
		double end_ecc = ConvertEccentricFromTrue(end_true_anomaly, elements.eccentricity);

		// средняя угловая скорость движения по орбите
		double mean_velocity = orbit.GetMeanVelocity();

		double temp = begin_ecc;

		// решаем уравнение Кеплера, [источник ?]
		while (temp < end_ecc)
		{
			// Интегрирование методом Эйлера
			// E = E0 + D / DT * [ mean_velocity / ( 1 - eccentricity * cos(E0)] 
			temp += step * mean_velocity / ( 1 - elements.eccentricity * cos(temp) );
			duration += step;
		}
	}

	///реализация конструктора активного участка (maneuver.h)
	CActivePath::CActivePath(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_acc,
			maneuver_type _type): orbit(_orbit),
			begin_true_anomaly(_begin_true_anomaly),
			step(_step),
			engine_time(_engine_time),
			engine_acc(_engine_acc),
			type(_type)
	{ }

	///реализация конструктора активного участка  с постоянным аргументом перицентра (maneuver.h)
	CActivePlanConstArg::CActivePlanConstArg(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_force,
			maneuver_type _type,
			double _constraint):CActivePath(_orbit, _begin_true_anomaly, _step, _engine_time, _engine_force, _type)
	{
		kepler_orbit elements = orbit.GetKeplerOrbitFormat();
		elements.true_anomaly = _begin_true_anomaly;

		history.push_back(elements);

		double temp_t = 0;
		//Моделирование движения (оскулирующие элементы - интегрируем Эйлером)
		while (temp_t < engine_time)
		{
			kepler_orbit curr_elements;

			double acc_r = engine_acc * sin( _engine_angle( elements.eccentricity, elements.true_anomaly ) );
			double acc_n = engine_acc * cos( _engine_angle( elements.eccentricity, elements.true_anomaly ) );

			curr_elements.eccentricity = elements.eccentricity + step * dif_ecc_proj(elements.true_anomaly, acc_r, acc_n, &elements);
			curr_elements.focal = elements.focal + step * dif_focal_proj(elements.true_anomaly, acc_r, acc_n, &elements);
			curr_elements.pericenter_angle = elements.pericenter_angle + step * dif_pericenter_ang_proj(elements.true_anomaly, acc_r, acc_n, &elements);
			curr_elements.true_anomaly = elements.true_anomaly + step * dif_tetta_proj(elements.true_anomaly, acc_r, acc_n, &elements);

			curr_elements.pericenter = curr_elements.focal / ( 1 + curr_elements.eccentricity);
			curr_elements.apocenter = curr_elements.focal / ( 1 - curr_elements.eccentricity);

			history.push_back(curr_elements);
			elements = curr_elements;
			temp_t += step;

			switch (type)
			{
			case maneuver::maneuver_complanar_apo_raise:
				if( _constraint >= elements.apocenter) break;

			case maneuver::maneuver_complanar_apo_descend:
				if( _constraint <= elements.apocenter) break;

			case maneuver::maneuver_complanar_peri_raise:
				if( _constraint >= elements.pericenter) break;

			case maneuver::maneuver_complanar_peri_descend:
				if( _constraint <= elements.pericenter) break;

			case maneuver::maneuver_complanar_peri_arg:
				break;
			default:
				break;
			}
		}
	}



	CSetApocenter::CSetApocenter(double apocenter, const CEllepticalOrbit& orbit) : 
		current_orbit(orbit),
		new_apocenter(apocenter)
	{
		kepler_orbit elements  = current_orbit.GetKeplerOrbitFormat();
		double target_apocenter = new_apocenter * 1000;

		if( target_apocenter > elements.apocenter ) 
			type = maneuver_complanar_apo_raise;
		else if (target_apocenter < elements.apocenter )
			type = maneuver_complanar_apo_descend;

	}

	std::vector<SPath*> CSetApocenter::GetTransferTrajectory()
	{
		std::vector<SPath*> result;
		switch(type)
		{
		case maneuver_complanar_apo_raise:
			{
				kepler_orbit elements  = current_orbit.GetKeplerOrbitFormat();
				double target_apocenter = new_apocenter * 1000;

				double curr_apocenter = elements.apocenter;
				while (curr_apocenter < target_apocenter )
				{
					//CActivePlanConstArg * active = new CActivePlanConstArg(
				}

			}
			break;
		case maneuver_complanar_apo_descend:
			break;
		default:
			break;
		}

		return result;
	}


	CSetPericenter::CSetPericenter(double pericenter, const CEllepticalOrbit& _current_orbit):
		current_orbit(_current_orbit),
		new_pericenter(pericenter)
	{

	}
}

namespace maneuver
{
//	CPassivePath::CPassivePath(){}
}
