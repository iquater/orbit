#include "orbit.h"

CPlanet::CPlanet(double planet_mass, double radius) 
	: m_planet_mass(planet_mass),
	m_radius(radius)
{

}

COrbit::COrbit(double planet_mass, double radius, double semi_major_axis, double eccentricity, double periapsis,
	double inclination, double ascending_node, double mean_anomaly) 
	:m_planet(planet_mass, radius),
	m_semi_major_axis(semi_major_axis),
	m_eccentricity(eccentricity),
	m_periapsis(periapsis),
	m_inclination(inclination),
	m_ascending_nod(ascending_node),
	m_mean_anomaly(mean_anomaly)

{

}

int main()
{
	return 0;
}