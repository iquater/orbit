#ifndef BODY_H
#define BODY_H
#include "constants.h"
class CBody
{
	double m_planet_mass; /// planet mass, kg
	double m_radius;			/// planet radius, km
	double m_min_height;			/// minimum orbit height, km

	CBody() {}
public:
	CBody(double planet_mass, double planet_radius);

	double GetPlanetMass(){ return m_planet_mass; }

	/**
	 * @brief возвращает массу планеты
	 */
	double GetPlanetRadius(){ return m_radius; }

	/**
	 * @brief возвращает минимальную высоту полета вне атмосферы ( для Земли 200км)
	 */
	double GetMinHeight(){ return m_min_height; }

	void SetMinHeight(int height){ m_min_height = height; }

	double GetGravyConst(){ return Gravy_const * m_planet_mass; }

};
#endif // !BODY_H
