#include "orbit.h"

CPlanet::CPlanet(double planet_mass, double radius) 
	: m_planet_mass(planet_mass),
	m_radius(radius)
{}

CEllepticalOrbit::CEllepticalOrbit(double planet_mass, double radius, double semi_major_axis, double eccentricity, double periapsis,
	double inclination, double ascending_node, double mean_anomaly) 
	:m_planet(planet_mass, radius),
	m_semi_major_axis(semi_major_axis),
	m_eccentricity(eccentricity),
	m_periapsis(periapsis),
	m_inclination(inclination),
	m_ascending_node(ascending_node),
	m_mean_anomaly(mean_anomaly)

{}


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

double CEllepticalOrbit::GetRadiusAtAnomaly(double anomaly)
{
	return GetFocal() / (1 + m_eccentricity * cos(anomaly));
}

double CEllepticalOrbit::EccentricToTrueAnomaly(double eccentric_anomaly)
{
	return atan2(sqrt(1 - m_eccentricity) * cos(eccentric_anomaly / 2),
		sqrt(1 + m_eccentricity) * sin(eccentric_anomaly / 2));
}

double CEllepticalOrbit::TrueToEccentricAnomaly(double anomaly)
{
	return atan(sqrt(1 - pow(m_eccentricity, 2)) * sin(anomaly) /
		(m_eccentricity + cos(anomaly)));
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
	return m_ascending_node;
}
int main()
{
	//first Earth mass, second mean Earth radius
	CEllepticalOrbit orbit(5.97219 * pow(10.0, 24.0), 6380000, 42164000, 0.1);
	double t = orbit.GetPeriod();
	double v = orbit.GetMeanVelocity(); 
	return 0;
}