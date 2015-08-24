#ifndef BODY_H
#define BODY_H

class CBody
{
	const double m_planet_mass; /// planet mass, kg
	const int m_radius;		/// planet radius, m

	int m_min_height; /// minimum orbit height, m

	CBody() :m_planet_mass(0), m_radius(0), m_min_height(0){}
	CBody(const CBody&) : m_planet_mass(0), m_radius(0), m_min_height(0){}
public:
	CBody(double planet_mass,
		int planet_radius
		);

	const double & GetPlanetMass() { return m_planet_mass; }

	const int & GetPlanetRadius(){ return m_radius; }

	const int & GetMinHeight(){ return m_min_height; }

	void SetMinHeight(int height) { m_min_height = height; }

};
#endif // !BODY_H
