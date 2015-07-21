#ifndef ORBIT_H
#define ORBIT_H
#define _USE_MATH_DEFINES
#include <math.h>

const double Gravy_const = 6.67408 * (pow(10.0, -11));
const double PiConst = M_PI;
class CPlanet
{
	const double m_planet_mass; /// масса планеты
	const double m_radius;		///радиус планеты

	CPlanet() :m_planet_mass(0), m_radius(0){}
	CPlanet(const CPlanet&) : m_planet_mass(0), m_radius(0){}
public:
	CPlanet(double planet_mass, ///масса планеты
		double radius			///радиус планеты
		);

	const double & GetPlanetMass() { return m_planet_mass; }

	const double & GetPlanetRadius(){ return m_radius; }
};

class CEllepticalOrbit
{
private:
	CPlanet m_planet;
	double m_semi_major_axis;	/// semi major axis, meters
	double m_eccentricity;		/// eccentricity
	double m_periapsis;			/// periapsis
	double m_inclination;		/// orbit inclination
	double m_ascending_node;	/// ascending node longitude
	double m_mean_anomaly;		/// Mean Anomaly
	double time;				/// time, that is corresponding to mean_anomaly
public:
	CEllepticalOrbit(double planet_mass, double radius, double semi_major_axis, double eccentricity,
		double periapsis = 0.0, double inclination = 0.0, double ascending_nod = 0.0, double mean_anomaly = 0.0);
private:

	CEllepticalOrbit(const CEllepticalOrbit&);

	CEllepticalOrbit();

public:
	/// return period of orbit
	double GetPeriod();

	double GetMeanVelocity();

	double GetFocal();

	double GetRadiusAtAnomaly(double anomaly);

	double GetApsis();

	double GetPeriapsis();

	double GetEccentricity();

	double GetInclination();

	double GetAscendingNode();

	double GetSemiMajorAxis();
	
	double EccentricToTrueAnomaly(double eccentric_anomaly);

	double TrueToEccentricAnomaly(double anomaly);

};


#endif //ORBIT_H