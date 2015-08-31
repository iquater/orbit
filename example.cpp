#include "orbit.h"
#include "constants.h"
#include "utilites.h"
#include <math.h>
#include <vector>
using namespace utilites;

/*Elements_f dif_motion[] = //система дифференциальных уравнений
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

typedef std::vector<step> history_t;*/

int main ()
{
	CEllepticalOrbit * orbit = new CEllepticalOrbit(EarthMass, 6400, 7200, 7600);
	CEllepticalOrbit * target_orbit = new CEllepticalOrbit(EarthMass, 6400, 7200, 10000);

	kepler_orbit ko = orbit->GetKeplerOrbitFormat();
	kepler_orbit nko = target_orbit->GetKeplerOrbitFormat();

	kepler_orbit new_co;
	new_co.eccentricity = ko.eccentricity +  dif_ecc(0, PiConst / 2, 200, &ko);
	new_co.focal = ko.focal + dif_focal(0, PiConst / 2, 200, &ko );

	double t = velocity_raise_apo(&ko, 10000000);
	double nvn = sqrt(EarthGravy/nko.focal * (1 + nko.eccentricity * cos(PiConst)) );

	history_t history;
	history.push_back(ko);
	int step = 60;
	double engine_dir = PiConst / 2;
	double engine_acc = 2;
	for (size_t i = 0; i < 10; i++)
	{ 
		kepler_orbit temp;
		temp.eccentricity = ko.eccentricity + step * dif_ecc(ko.true_anomaly, engine_dir, engine_acc, &ko);
		temp.focal = ko.focal + step * dif_focal(ko.true_anomaly, engine_dir, engine_acc, &ko);
		temp.pericenter_angle = ko.pericenter_angle + step * dif_periapsis_arg(ko.true_anomaly, engine_dir, engine_acc, &ko);
		temp.pericenter = ko.pericenter + step * dif_periapsis(ko.true_anomaly, engine_dir, engine_acc, &ko);
		temp.apocenter = ko.apocenter + step * dif_apsis(ko.true_anomaly, engine_dir, engine_acc, &ko);
		temp.true_anomaly = ko.true_anomaly + step * dif_tetta(ko.true_anomaly, engine_dir, engine_acc, &ko);
		history.push_back(temp);
		ko = temp;
	}



	while(getchar()!='\n')
	{;}
	return 0;
}
