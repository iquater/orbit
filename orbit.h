#ifndef ORBIT_H
#define ORBIT_H

#include "body.h"
#include <vector>


/// структура данных для хранения значений оскулирующих элементов орбиты
struct kepler_orbit
{
	double pericenter;
	double apocenter;
	double eccentricity;
	double focal;
	double pericenter_angle;
	double true_anomaly;

	kepler_orbit():
		pericenter(0),
		apocenter(0),
		eccentricity(0),
		focal(0),
		pericenter_angle(0),
		true_anomaly(0) {}

	bool operator == (kepler_orbit const & orbit) const
	{
		return this->pericenter == orbit.pericenter && this->apocenter == orbit.apocenter 
			&& this->eccentricity == orbit.eccentricity && this->focal == orbit.focal 
			&& this->pericenter_angle == orbit.pericenter_angle;
	}

};

typedef std::vector<kepler_orbit> history_t;

class CEllepticalOrbit
{
	CBody m_planet; /// планета, в окрестностях которой строится орбита

	/// параметры, определяющие форму орбиты
	double m_pericenter; /// радиус перицентра орбиты, [км]
	double m_apocenter; ///  радиус апоцентра орбиты, [км]
	double m_focal; /// фокальный параметр, [км]
	double m_semi_major_axis; /// большая полуось, [км]
	double m_eccentricity; /// эксцентриситет

	/// параметры, определяющие угловое положение орбиты относительно планеты:
	double m_inclination; /// наклонение орбиты, [рад]
	double m_ascending_node; /// долгота восходящего узла, [рад]
	double m_pericenter_ang; /// аргумент перицентра, [рад]

public:
	CEllepticalOrbit(double planet_mass, double planet_radius, 
		double pericenter, 
		double apocenter,
		double inclination = 0.0,
		double ascending_node = 0.0 ,
		double pericenter_ang = 0.0);


	kepler_orbit GetKeplerOrbitFormat()
	{
		kepler_orbit ko;
		ko.pericenter = m_pericenter * 1000;
		ko.apocenter = m_apocenter * 1000;
		ko.eccentricity = m_eccentricity;
		ko.focal = m_focal * 1000;
		ko.pericenter_angle = m_pericenter_ang;
		ko.true_anomaly = 0.0;

		return ko;
	}

	double GetOrbitPeriod();

	double GetMeanVelocity();

	CBody GetPlanet() { return m_planet; }
};


#endif //ORBIT_H

/*class CEllepticalOrbit
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

	double GetPeriapsisAng();
};*/