#include "orbit.h"
#include "constants.h"
#include "utilites.h"
#include <math.h>
#include <vector>
using namespace utilites;

Elements_f dif_motion[] = //система дифференциальных уравнений
{ 
	dif_ecc, 
	dif_focal, 
	dif_periapsis_arg, 
	dif_periapsis, 
	dif_apsis, 
	dif_tetta 
};


struct step
{
	double eccentricity;
	double focal;
	double periapsis_angle;
	double periapsis;
	double apsis;
	double true_anomaly;

	step():eccentricity(0),
		focal(0),
		periapsis_angle(0),
		periapsis(0),
		apsis(0),
		true_anomaly(0) {}
};

typedef std::vector<step> history_t;

int main ()
{
	CEllepticalOrbit * orbit = new CEllepticalOrbit(EarthMass, 6400000, 7200000, 7600000);
	utilites::DebugPrintOrbit(*orbit);
	CEllepticalOrbit* target_orbit = new CEllepticalOrbit(EarthMass, 6400000, 7200000, 7800000);
	utilites::DebugPrintOrbit(*target_orbit);


	double init_condition[] =
	{
		orbit->GetEccentricity(),
		orbit->GetFocal(),
		orbit->GetPeriapsisAng(),
		orbit->GetPeriapsis(),
		orbit->GetApsis(),
		0
	};

	step init_step;
	init_step.eccentricity = orbit->GetEccentricity();
	init_step.focal = orbit->GetFocal();
	init_step.periapsis_angle = orbit->GetPeriapsisAng();
	init_step.periapsis = orbit->GetPeriapsis();
	init_step.apsis = orbit->GetApsis();
	init_step.true_anomaly = 0;

	history_t history;
	history.reserve(1000);
	history.push_back(init_step);

	step prev = init_step;
	double force = 2000, step_i = 0.0001, direction = PiConst/2;
	while (fabs(prev.apsis/1000 - target_orbit->GetApsis()/1000) > 0.1)
	{
		step curr;
		CEllepticalOrbit temp(EarthGravy, 6400000, (int)prev.periapsis, (int)prev.apsis, prev.periapsis_angle);

		curr.eccentricity = prev.eccentricity + step_i * dif_ecc(prev.true_anomaly, direction, force, &temp);
		curr.focal = prev.focal + step_i * dif_focal(prev.true_anomaly, direction, force, &temp);
		curr.periapsis_angle = prev.periapsis_angle + step_i * dif_periapsis_arg(prev.true_anomaly, direction, force, &temp);
		curr.periapsis = prev.periapsis + step_i * dif_periapsis(prev.true_anomaly, direction, force, &temp);
		curr.apsis = prev.apsis + step_i * dif_apsis(prev.true_anomaly, direction, force, &temp);
		curr.true_anomaly = prev.true_anomaly + step_i * dif_tetta( prev.true_anomaly, direction, force, &temp);
		prev = curr;
		history.push_back(curr);
	}


	while(getchar()!='\n')
	{;}
	return 0;
}
