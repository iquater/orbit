#ifndef ORBIT_H
#define ORBIT_H

const double Gravy_const = 6.67408 * (10 ^ (-11));

class CPlanet
{
	const double m_planet_mass; /// масса планеты
	const double m_radius;		///радиус планеты
public:
	CPlanet(double planet_mass, ///масса планеты
		double radius			///радиус планеты
		);

	const double & GetPlanetMass() { return m_planet_mass; }

	const double & GetPlanetRadius(){ return m_radius; }
};

class COrbit
{
private:
	CPlanet m_planet;
	double m_semi_major_axis;	/// больша€ полуось
	double m_eccentricity;		/// эксцентриситет орбиты
	double m_periapsis;			/// аргумент перге€
	double m_inclination;		/// наклонение орбиты
	double m_ascending_nod;		/// долгота восход€щего узла
	double m_mean_anomaly;		/// средн€€ аномали€
public:
	COrbit(double planet_mass, double radius, double semi_major_axis, double eccentricity, 
		double periapsis = 0.0, double inclination = 0.0, double ascending_nod = 0.0, double mean_anomaly = 0.0);
private:

	COrbit(const COrbit&);

	COrbit();

public:

	double GetPeriod();



};


#endif //ORBIT_H