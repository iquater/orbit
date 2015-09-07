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
			double _engine_acc): orbit(_orbit),
			begin_true_anomaly(_begin_true_anomaly),
			step(_step),
			engine_time(_engine_time),
			engine_acc(_engine_acc)
	{ }

	///реализация конструктора активного участка  с постоянным аргументом перицентра (maneuver.h)
	CActivePlanConstArg::CActivePlanConstArg(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_force):CActivePath(_orbit, _begin_true_anomaly, _step, _engine_time, _engine_force)
	{
		kepler_orbit elements = orbit.GetKeplerOrbitFormat();
		elements.true_anomaly = _begin_true_anomaly;

		history.push_back(elements);

		double temp_t = 0;
		//Моделирование движения (оскулирующие элементы - интегрируем Эйлером)
		while (temp_t <= engine_time)
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
		}
	}



	CSetApocenter::CSetApocenter(double apocenter, const CEllepticalOrbit& orbit) : 
		current_orbit(orbit),
		new_apocenter(apocenter)
	{

	}

	void CSetApocenter::Simulate()
	{
		double t_engine = 7; // время работы двигателя
		double engine_force = 19000; // тяга двигателя (Разгонный Блок), Н
		double mass = 7000;
		kepler_orbit tko = current_orbit.GetKeplerOrbitFormat();

		double engine_acc = engine_force / mass;

		double curr_apo = tko.apocenter;
		CEllepticalOrbit orbit(current_orbit);
		std::vector<history_t> whole_hist;
		while (curr_apo < new_apocenter * 1000 )
		{ 
			double dt = 1, summ_t = 0;
			kepler_orbit ko = orbit.GetKeplerOrbitFormat();
			history_t history;
			history.push_back(ko);
			while (summ_t <= t_engine)
			{ 
				kepler_orbit temp;
				double acc_r = engine_acc * sin(atan(tan(ko.true_anomaly) * 
					(1 + 1 / (1 + tko.eccentricity * cos(tko.true_anomaly)))));

				double acc_n = engine_acc * cos(atan(tan(tko.true_anomaly) * 
					(1 + 1 / (1 + tko.eccentricity * cos(tko.true_anomaly)))));

				temp.eccentricity = tko.eccentricity + dt * dif_ecc_proj(ko.true_anomaly, acc_r, acc_n, &ko);
				temp.focal = ko.focal + dt * dif_focal_proj(ko.true_anomaly, acc_r, acc_n, &ko);
				temp.pericenter_angle = ko.pericenter_angle + dt * dif_pericenter_ang_proj(ko.true_anomaly, acc_r, acc_n, &ko);
				temp.true_anomaly = ko.true_anomaly + dt * dif_tetta_proj(ko.true_anomaly, acc_r, acc_n, &ko);

				summ_t += t_engine;

				temp.pericenter = temp.focal / (1 + temp.eccentricity);
				temp.apocenter = temp.focal / (1 - temp.eccentricity);

				history.push_back(temp);
				ko = temp;
			}
			whole_hist.push_back(history);

			CEllepticalOrbit new_orb(orbit.GetPlanet().GetPlanetMass(),
				orbit.GetPlanet().GetPlanetRadius(),
				ko.pericenter / 1000, ko.apocenter / 1000);
			curr_apo = ko.apocenter;
			orbit = new_orb;
		}
	}


	CSetPericenter::CSetPericenter(double pericenter, const CEllepticalOrbit& _current_orbit):
		current_orbit(_current_orbit),
		new_pericenter(pericenter)
	{

	}

	void CSetPericenter::Simulate()
	{
		double t_engine = 7; // 7 секунд

	}
}

namespace maneuver
{
//	CPassivePath::CPassivePath(){}
}
