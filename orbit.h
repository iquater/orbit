#ifndef ORBIT_H
#define ORBIT_H

#include <iostream>

#include "body.h"




class CEllepticalOrbit
{
private:
	CBody m_planet;
	double m_semi_major_axis;	/// semi major axis, meters
	double m_eccentricity;		/// eccentricity
	double m_periapsis_ang;		/// periapsis
	double m_inclination;		/// orbit inclination
	double m_ascending_node;	/// ascending node longitude
	double m_mean_anomaly;		/// Mean Anomaly
	double m_anomaly;			/// True Anomaly
	double m_periapsis;			/// periapsis
	double m_apsis;				/// apsis
	double time;				/// time, that is corresponding mean_anomaly or True anomaly
public:
	/// constructor with semi_major_axis & eccentricity
	CEllepticalOrbit(double planet_mass, int radius, double semi_major_axis, double eccentricity,
		double periapsis_ang = 0.0, double inclination = 0.0, double ascending_node = 0.0, double mean_anomaly = 0.0);

	/// constructor with apsis & periapsis
	/// problems with constructor override - that's why i'm using long int.
	CEllepticalOrbit(double planet_mass, int radius, int periapsis, int apsis,
		double periapsis_ang = 0.0, double inclination = 0.0, double ascending_node = 0.0, double mean_anomaly = 0.0);
private:

	CEllepticalOrbit(const CEllepticalOrbit&);

	CEllepticalOrbit();

public:
	double GetPeriod();

	double GetMeanVelocity();

	double GetFocal();
	
	void SetFocal(double );

	double GetRadiusAtAnomaly(double anomaly);

	double GetApsis();

	double GetPeriapsis();

	double GetEccentricity();

	double GetInclination();

	double GetAscendingNode();

	double GetSemiMajorAxis();
};


#endif //ORBIT_H
