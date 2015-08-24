#include "orbit.h"
#include <stdlib.h>
#include <vector>
#include "constants.h"

CEllepticalOrbit::CEllepticalOrbit(double planet_mass, int radius, double semi_major_axis, double eccentricity, double periapsis_ang,
	double inclination, double ascending_node, double mean_anomaly) 
	:m_planet(planet_mass, radius),
	m_semi_major_axis(semi_major_axis),
	m_eccentricity(eccentricity),
	m_periapsis_ang(periapsis_ang),
	m_inclination(inclination),
	m_ascending_node(ascending_node),
	m_mean_anomaly(mean_anomaly),
	m_anomaly(0),
	m_periapsis((1 - eccentricity) * semi_major_axis),
	m_apsis((1 + eccentricity) * semi_major_axis)

{

}

CEllepticalOrbit::CEllepticalOrbit(double planet_mass, int radius, int periapsis, int apsis,
		double periapsis_ang, double inclination, double ascending_node, double mean_anomaly)
	:m_planet(planet_mass, radius),
	m_semi_major_axis((periapsis + apsis) / 2),
	m_eccentricity( 0),
	m_periapsis_ang(periapsis_ang),
	m_inclination(inclination),
	m_ascending_node(ascending_node),
	m_mean_anomaly(mean_anomaly),
	m_anomaly(0),
	m_periapsis(periapsis),
	m_apsis(apsis)
{
	if ( periapsis > apsis )
	{
		double tmp;
		tmp = apsis;
		apsis = periapsis;
		periapsis = tmp;
	}
	else if (periapsis == apsis) 
		m_eccentricity = 0;

	periapsis /= 1000;
	apsis /= 1000;

	double semi_major_axis = m_semi_major_axis / 1000;

	double ea = apsis / semi_major_axis - 1;

	m_eccentricity = ea;
}


double CEllepticalOrbit::GetPeriod()
{
	return 2 * PiConst * sqrt(powl(m_semi_major_axis, 3)) / sqrt(m_planet.GetPlanetMass() * Gravy_const);
}

double CEllepticalOrbit::GetMeanVelocity()
{
	return 2 * PiConst / GetPeriod();
}

double CEllepticalOrbit::GetFocal()
{
	return m_semi_major_axis * (1 - pow(m_eccentricity, 2) );
}

void CEllepticalOrbit::SetFocal(double focal)
{
	m_semi_major_axis = focal / (1 - pow(m_eccentricity, 2));
}
double CEllepticalOrbit::GetRadiusAtAnomaly(double anomaly)
{
	return GetFocal() / (1 + m_eccentricity * cos(anomaly));
}

double CEllepticalOrbit::GetApsis()
{
	return (1 + m_eccentricity) * m_semi_major_axis;
}

double CEllepticalOrbit::GetPeriapsis()
{
	return (1 - m_eccentricity) * m_semi_major_axis;
}
double CEllepticalOrbit::GetEccentricity()
{
	return m_eccentricity;
}

double CEllepticalOrbit::GetInclination()
{
	return m_inclination;
}

double CEllepticalOrbit::GetAscendingNode()
{
	return m_ascending_node;
}

double CEllepticalOrbit::GetSemiMajorAxis()
{
	return m_semi_major_axis;
}


void increaseApsis(CEllepticalOrbit * orbit, int new_apsis)
{
	double f = 200;
	//std::vector<CEllepticalOrbit *> transformation;

	
}

double dif_ecc(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// de/dt:
	double a = engine_force * sqrt(orbit->GetFocal() / EarthGravy);
	double b = sin(anomaly)* cos(engine_direction);
	double c = orbit->GetEccentricity() * cos(anomaly) * cos(anomaly) + 2 * cos(anomaly) + orbit->GetEccentricity();
	double d = 1 + orbit->GetEccentricity()*cos(anomaly);
	double e = sin(engine_direction);

	result = a * (b + c * e / d);

	return result;
}

double dif_focal(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dp/dt:

	double a = 2 * engine_force * orbit->GetFocal();
	double b = 1 + orbit->GetEccentricity() * cos(anomaly);
	double c = sqrt(orbit->GetFocal() / EarthGravy) * sin(engine_direction);

	result = a * c / b;

	return result;
}

double dif_periapsis_arg(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dw/dt:	

	double a = engine_force / orbit->GetEccentricity() * sqrt(orbit->GetFocal() / EarthGravy);
	double b = cos(anomaly) * cos(engine_direction);
	double c = (2 + orbit->GetEccentricity() * cos(anomaly)) * sin(anomaly);
	double d = sin(engine_direction) / (1 + orbit->GetEccentricity() * cos(anomaly));

	result = a * (-b + c * d);

	return result;
}

double dif_periapsis(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dperiapsis/dt:	
	double a = engine_force * orbit->GetFocal()  * sqrt(orbit->GetFocal() / EarthGravy);
	double b = 1 + orbit->GetEccentricity() * orbit->GetEccentricity();
	double c = sin(anomaly) * cos(engine_direction);
	double d = sin(engine_direction) / (1 + orbit->GetEccentricity() * cos(anomaly));
	double e = 2 * (1 - cos(anomaly)) + orbit->GetEccentricity() * sin(anomaly) * sin(anomaly);

	result = a / b *(-c - e * d);

	return result;
}

double dif_apsis(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dperiapsis/dt:	
	double a = engine_force * orbit->GetFocal()  * sqrt(orbit->GetFocal() / EarthGravy);
	double b = 1 - orbit->GetEccentricity() * orbit->GetEccentricity();
	double c = sin(anomaly) * cos(engine_direction);
	double d = sin(engine_direction) / (1 + orbit->GetEccentricity() * cos(anomaly));
	double e = 2 * (1 + cos(anomaly)) - orbit->GetEccentricity() * sin(anomaly) * sin(anomaly);

	result = a / b *(c - e * d);

	return result;
}

double dif_tetta(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	double r = (orbit->GetFocal() / (1 + orbit->GetEccentricity() * cos(anomaly)));
	double a = sqrt(EarthGravy * orbit->GetFocal()) /
		pow(r , 2);

	double b = engine_force * cos(anomaly) / orbit->GetEccentricity();
	double c = sqrt(orbit->GetFocal() / EarthGravy) * cos(engine_direction);
	double d = engine_direction * sin(anomaly) / orbit->GetEccentricity() * (1 + r / orbit->GetFocal());
	double f = sqrt(orbit->GetFocal() / EarthGravy) * sin(engine_direction);

	result = a + b * c - d * f;


	return result;
}
typedef double(*Elements_f)(double, double, double, CEllepticalOrbit *);
Elements_f dif_motion[] = //система дифференциальных уравнений
{ 
	dif_ecc, 
	dif_focal, 
	dif_periapsis_arg, 
	dif_periapsis, 
	dif_apsis, 
	dif_tetta 
};
void fa()
{
	//задаем начальную орбиту
	CEllepticalOrbit orbit(5.97219 * pow(10.0, 24.0), 6380000.0, 7800000.0, 0.1);

	int t = 0, end = 10;
	double fource = 0, force_direction = 0;
	double step = 0.1;
	double anomaly = 0;
	while (t < end)
	{ 
		double e1 = orbit.GetEccentricity() + step * dif_ecc(anomaly, 0, 100, &orbit);
		double f1 = orbit.GetFocal() + step * dif_focal(anomaly, 0, 100, &orbit);
		double parg1 = orbit.GetPeriapsis() + step * dif_periapsis_arg(anomaly, 0, 100, &orbit);
		double p1 = orbit.GetEccentricity() + step * dif_ecc(anomaly, 0, 100, &orbit);
	
	}
}
