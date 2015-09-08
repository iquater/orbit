#include "body.h"

CBody::CBody(double planet_mass, double radius)
	: m_planet_mass(planet_mass),
	m_radius(radius),
	m_min_height(0)
{}