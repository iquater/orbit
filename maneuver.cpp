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
			type(_type),
			duration(0.0)
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
				if( _constraint <= elements.apocenter)
					return;
				break;

			case maneuver::maneuver_complanar_apo_descend:
				if( _constraint >= elements.apocenter) 
					return;
				break;

			case maneuver::maneuver_complanar_peri_raise:
				if( _constraint <= elements.pericenter) 
					return;
				break;

			case maneuver::maneuver_complanar_peri_descend:
				if( _constraint >= elements.pericenter) 
					return;
				break;

			case maneuver::maneuver_complanar_peri_arg:
				break;
			default:
				break;
			}
		}
	}

	const kepler_orbit & CActivePlanConstArg::get_finish_pos()
	{
		return history.at( history.size() - 1);
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

	std::vector<SPath*> CSetApocenter::GetTransferTrajectory(double true_anomaly, double eng_time, double eng_acc, double step)
	{
		std::vector<SPath*> result;

		kepler_orbit elements  = current_orbit.GetKeplerOrbitFormat();
		double target_apocenter = new_apocenter * 1000;

		double curr_apocenter = elements.apocenter;
		CEllepticalOrbit temp_orbit(current_orbit);

		double constraint = new_apocenter  * 1000;
		long double final_time = 0.0;
		
		//if(true_anomaly != 0.0)

		switch(type)
		{
		case maneuver_complanar_apo_raise:
			{
				while (curr_apocenter < target_apocenter )
				{
					//перевод начинается с активного участка в перицентре орбиты
					CActivePlanConstArg * active = new CActivePlanConstArg(temp_orbit, true_anomaly, step, eng_time, eng_acc, type, constraint );
					const kepler_orbit & finish_pos = active->get_finish_pos();
					curr_apocenter = finish_pos.apocenter;
					result.push_back(active);

					if (curr_apocenter >= target_apocenter)
					{
						// если достигли требуемого значения апоцентра - вычисляем время
						size_t n = active->history.size();
						double dt = (n - 1) * step;
						final_time +=dt;
						active->duration = dt;
						break;
					}
					active->duration = eng_time;
					final_time += eng_time;
					// пассивный учаток - виток до нового включения двигателей
					CEllepticalOrbit orb(temp_orbit.GetPlanet().GetPlanetMass(), temp_orbit.GetPlanet().GetPlanetRadius(), 
						finish_pos.pericenter / 1000, finish_pos.apocenter / 1000);
					temp_orbit = orb;
					CPassivePath * pass = new CPassivePath(orb, finish_pos.true_anomaly, 2 * PiConst, step);
					final_time += pass->duration;
					result.push_back(pass);
				}
				return result;
			}
			break;
		case maneuver_complanar_apo_descend:
			{
				while (curr_apocenter > target_apocenter )
				{
					CActivePlanConstArg * active = new CActivePlanConstArg(temp_orbit, true_anomaly, step, eng_time, eng_acc * (-1), type, constraint );
					const kepler_orbit & finish_pos = active->get_finish_pos();
					
					CEllepticalOrbit orb(temp_orbit.GetPlanet().GetPlanetMass(), temp_orbit.GetPlanet().GetPlanetRadius(), 
						finish_pos.pericenter / 1000, finish_pos.apocenter / 1000);
					curr_apocenter = finish_pos.apocenter;
					result.push_back(active);

					if(curr_apocenter <= target_apocenter)
					{
						size_t n = active->history.size();
						double dt = (n - 1) * step;
						final_time +=dt;
						active->duration = dt;
						break;
					}
					active->duration = eng_time;
					final_time += eng_time;

					CPassivePath * pass = new CPassivePath(orb, finish_pos.true_anomaly, 2 * PiConst, step);
					final_time += pass->duration;
				
					temp_orbit = orb;
					result.push_back(pass);
				}
				return result;
			}
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
		kepler_orbit elements  = current_orbit.GetKeplerOrbitFormat();
		double target_pericenter = new_pericenter * 1000;

		if( target_pericenter > elements.pericenter ) 
			type = maneuver_complanar_peri_raise;
		else if (target_pericenter < elements.pericenter )
			type = maneuver_complanar_peri_descend;
	}

	std::vector<SPath*> CSetPericenter::GetTransferTrajectory(double true_anomaly, double eng_time, double eng_acc, double step)
	{
		
		std::vector<SPath*> result;

		kepler_orbit elements  = current_orbit.GetKeplerOrbitFormat();
		double target_pericenter = new_pericenter * 1000;

		double curr_pericenter = elements.pericenter;
		CEllepticalOrbit temp_orbit(current_orbit);

		double constraint = new_pericenter  * 1000;
		long double final_time = 0.0;

		true_anomaly = PiConst;//активный участок начинается в апоцентре

		switch(type)
		{
		case maneuver_complanar_peri_raise:
			{
				while (curr_pericenter <  target_pericenter)
				{
					//перевод начинается с активного участка в апоцентре орбиты
					CActivePlanConstArg * active = new CActivePlanConstArg(temp_orbit, true_anomaly, step, eng_time, eng_acc, type, constraint );
					const kepler_orbit & finish_pos = active->get_finish_pos();
					curr_pericenter = finish_pos.pericenter;
					result.push_back(active);

					if (curr_pericenter >= target_pericenter)
					{
						// если достигли требуемого значения перицентра - вычисляем время
						size_t n = active->history.size();
						double dt = (n - 1) * step;
						final_time +=dt;
						active->duration = dt;
						break;
					}
					active->duration = eng_time;
					final_time += eng_time;
					// пассивный учаток - виток до нового включения двигателей
					CEllepticalOrbit orb(temp_orbit.GetPlanet().GetPlanetMass(), temp_orbit.GetPlanet().GetPlanetRadius(), 
						finish_pos.pericenter / 1000, finish_pos.apocenter / 1000);
					temp_orbit = orb;
					CPassivePath * pass = new CPassivePath(orb, finish_pos.true_anomaly, 2 * PiConst, step);
					final_time += pass->duration;
					result.push_back(pass);
				}
				return result;
			}
			break;
		case maneuver_complanar_peri_descend:
			{
				while (curr_pericenter >  target_pericenter)
				{
					//перевод начинается с активного участка в апоцентре орбиты
					CActivePlanConstArg * active = new CActivePlanConstArg(temp_orbit, true_anomaly, step, eng_time, eng_acc*(-1), type, constraint );
					const kepler_orbit & finish_pos = active->get_finish_pos();
					curr_pericenter = finish_pos.pericenter;
					result.push_back(active);

					if (curr_pericenter <= target_pericenter)
					{
						// если достигли требуемого значения перицентра - вычисляем время
						size_t n = active->history.size();
						double dt = (n - 1) * step;
						final_time +=dt;
						active->duration = dt;
						break;
					}
					active->duration = eng_time;
					final_time += eng_time;
					// пассивный учаток - виток до нового включения двигателей
					CEllepticalOrbit orb(temp_orbit.GetPlanet().GetPlanetMass(), temp_orbit.GetPlanet().GetPlanetRadius(), 
						finish_pos.pericenter / 1000, finish_pos.apocenter / 1000);
					temp_orbit = orb;
					CPassivePath * pass = new CPassivePath(orb, finish_pos.true_anomaly, 2 * PiConst, step);
					final_time += pass->duration;
					result.push_back(pass);
				}
				return result;
			}
			break;
		default:
			return result;
		}
	}

}

namespace maneuver
{
//	CPassivePath::CPassivePath(){}
}
