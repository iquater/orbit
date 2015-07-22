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

	double m_min_height;

	CPlanet() :m_planet_mass(0), m_radius(0),m_min_height(0){}
	CPlanet(const CPlanet&) : m_planet_mass(0), m_radius(0), m_min_height(0){}
public:
	CPlanet(double planet_mass, ///масса планеты
		double radius			///радиус планеты
		);

	const double & GetPlanetMass() { return m_planet_mass; }

	const double & GetPlanetRadius(){ return m_radius; }

	const double & GetMinHeight(){ return m_min_height; }

	void SetMinHeight(double height) { m_min_height = height; }



};

class CEllepticalOrbit
{
private:
	CPlanet m_planet;
	double m_semi_major_axis;	/// semi major axis, meters
	double m_eccentricity;		/// eccentricity
	double m_periapsis_ang;		/// periapsis
	double m_inclination;		/// orbit inclination
	double m_ascending_node;	/// ascending node longitude
	double m_mean_anomaly;		/// Mean Anomaly
	double time;				/// time, that is corresponding mean_anomaly
public:
	/// constructor with semi_major_axis & eccentricity
	CEllepticalOrbit(double planet_mass, double radius, double semi_major_axis, double eccentricity,
		double periapsis_ang = 0.0, double inclination = 0.0, double ascending_node = 0.0, double mean_anomaly = 0.0);

	/// constructor with apsis & periapsis
	/// problems with constructor override - that's why i'm using long int.
	CEllepticalOrbit(double planet_mass, double radius, long int periapsis, long int apsis,
		double periapsis_ang = 0.0, double inclination = 0.0, double ascending_node = 0.0, double mean_anomaly = 0.0);
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
