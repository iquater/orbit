#ifndef BODY_H
#define BODY_H

class CBody
{
	const double m_planet_mass; /// planet mass
	const double m_radius;		/// planet radius

	double m_min_height;

	CBody() :m_planet_mass(0), m_radius(0), m_min_height(0){}
	CBody(const CBody&) : m_planet_mass(0), m_radius(0), m_min_height(0){}
public:
	CBody(double planet_mass,
		double planet_radius
		);

	const double & GetPlanetMass() { return m_planet_mass; }

	const double & GetPlanetRadius(){ return m_radius; }

	const double & GetMinHeight(){ return m_min_height; }

	void SetMinHeight(double height) { m_min_height = height; }

};
#endif // !BODY_H
