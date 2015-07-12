#ifndef ORBIT_H
#define ORBIT_H


class CPlanet
{
	const double m_planet_mass; // масса планеты
	const double m_radius; //радиус планеты
public:
	CPlanet(double planet_mass, ///масса планеты
		double radius ///радиус планеты
		);

	const double & GetPlanetMass() { return m_planet_mass; }

	const double & GetPlanetRadius(){ return m_radius; }
};

class COrbit
{
private:
	CPlanet m_planet;
	double m_semi_major_axis; /// большая полуось
	double m_eccentricity; /// эксцентриситет орбиты
	double m_periapsis; //аргумент пергея
	double m_inclination;
	double m_ascending_nod;
	double m_mean_anomaly;
public:
	COrbit(double planet_mass, ///масса планеты
		double radius, ///радиус планеты
		double semi_major_axis, ///большая полуось
		double eccentricity, /// эксцентриситет орбты
		double periapsis = 0.0,
		double inclination = 0.0,
		double ascending_nod = 0.0,
		double mean_anomaly = 0.0);
private:

	COrbit(const COrbit&);

	COrbit();

public:

	double GetPeriod();



};


#endif //ORBIT_H