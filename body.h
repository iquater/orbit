#ifndef BODY_H
#define BODY_H

class CBody
{
	const double m_planet_mass; /// planet mass, kg
	const double m_radius;			/// planet radius, km
	double m_min_height;			/// minimum orbit height, km

	CBody() :m_planet_mass(0), m_radius(0), m_min_height(0){}

	CBody(const CBody&) : m_planet_mass(0), m_radius(0), m_min_height(0){}

public:
	CBody(double planet_mass, double planet_radius);

	const double & GetPlanetMass() { return m_planet_mass; }

	/**
	 * @brief возвращает массу планеты
	 */
	double GetPlanetRadius(){ return m_radius; }

	/**
	 * @brief возвращает минимальную высоту полета вне атмосферы ( для Земли 200км)
	 */
	double GetMinHeight(){ return m_min_height; }


	void SetMinHeight(int height) { m_min_height = height; }

};
#endif // !BODY_H
