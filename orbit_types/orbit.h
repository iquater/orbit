#ifndef ORBIT_H
#define ORBIT_H
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
const double Gravy_const = 6.67408 * (pow(10.0, -11));
const double PiConst = M_PI;
const double EarthMass = 5.97219 * (pow(10, 24));
const double EarthGravy = Gravy_const * EarthMass;

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


double dif_ecc( double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// de/dt:
	double a = engine_force * sqrt(orbit->GetFocal() / EarthGravy);
	double b = sin(anomaly)* cos(engine_direction);
	double c = orbit->GetEccentricity() * cos(anomaly) * cos(anomaly) + 2 * cos(anomaly) + orbit->GetEccentricity();
	double d = 1 + orbit->GetEccentricity()*cos(anomaly);
	double e = sin(engine_direction);

	result = a * (b + c * e / d);

	return result;
}

double dif_focal(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dp/dt:

	double a = 2 * engine_force * orbit->GetFocal();
	double b = 1 + orbit->GetEccentricity() * cos(anomaly);
	double c = sqrt(orbit->GetFocal() / EarthGravy) * sin(engine_direction);

	result = a * c / b;

	return result;
}

double dif_periapsis_arg(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dw/dt:	

	double a = engine_force / orbit->GetEccentricity() * sqrt(orbit->GetFocal() / EarthGravy);
	double b = cos(anomaly) * cos(engine_direction);
	double c = (2 + orbit->GetEccentricity() * cos(anomaly)) * sin(anomaly) ;
	double d = sin(engine_direction) / (1 + orbit->GetEccentricity() * cos(anomaly));

	result = a * (-b + c * d);

	return result;
}

double dif_periapsis(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dperiapsis/dt:	
	double a = engine_force * orbit->GetFocal()  * sqrt(orbit->GetFocal() / EarthGravy);
	double b = 1 + orbit->GetEccentricity() * orbit->GetEccentricity();
	double c = sin(anomaly) * cos(engine_direction);
	double d = sin(engine_direction) / (1 + orbit->GetEccentricity() * cos(anomaly));
	double e = 2 * (1 - cos(anomaly)) + orbit->GetEccentricity() * sin(anomaly) * sin(anomaly); 

	result = a / b *(-c - e * d);

	return result;
}

double dif_apsis(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit)
{
	double result = 0;
	/// dperiapsis/dt:	
	double a = engine_force * orbit->GetFocal()  * sqrt(orbit->GetFocal() / EarthGravy);
	double b = 1 - orbit->GetEccentricity() * orbit->GetEccentricity();
	double c = sin(anomaly) * cos(engine_direction);
	double d = sin(engine_direction) / (1 + orbit->GetEccentricity() * cos(anomaly));
	double e = 2 * (1 + cos(anomaly)) - orbit->GetEccentricity() * sin(anomaly) * sin(anomaly);

	result = a / b *(c - e * d);

	return result;
}
#endif //ORBIT_H
