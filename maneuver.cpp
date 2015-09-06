#include "maneuver.h"
#include "utilites.h"

using namespace utilites;

using namespace maneuver;

namespace maneuver
{
	/*CSetApsis::CSetApsis(double new_apsis, const CEllepticalOrbit& orbit)
	{
	double old_apsis = orbit.GetApsis();
	if (old_apsis < new_apsis)
	{


	}
	}*/
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