#ifndef SATELLITE_H
#define SATELLITE_H

#include "orbit.h"
#include <map>

using namespace std;
struct AccelerationComponent
{
	double transversal;
	double radial;

	AccelerationComponent():transversal(.0), radial(.0){};
	}

};


class ISatellite
{
public:
	///виртуальный деструктор
	virtual ~ISatellite(){}

	/// ¬ернуть ћассу —путника
	virtual double GetMass() = 0; // кг

	/// получить орбиту спутника
	virtual const CEllepticalOrbit & GetSatelliteOrbit() = 0;

	/// получить компоненты (трансверсальна€, радиальна€) ускорени€ двигател€ в заданной точке орбиты
	virtual AccelerationComponent GetAccelerationComponent(double true_anomaly) = 0;

	/// сменить закон управлени€ двигателем
	/// параметр law по сути представл€ет номер закона управлени€,
	/// который определ€етс€ пользователем класса
	virtual void SwitchControlLaw(int law) = 0;

	virtual int GetControlLaw() const = 0 ;

	/// ”становить новые параметры орбиты спутника
	virtual void SetOrbit(const kepler_orbit & orbit) = 0;
	virtual void SetOrbit(const CEllepticalOrbit & orbit) = 0;
};

class CDefaultSatellite: public ISatellite
{
	double m_mass; // кг
	double m_engine_acc; // м/с^2
	CEllepticalOrbit m_orbit;
	int m_control_law;

	

	enum type_eng_law
	{
		type_eng_const_periarg = 0,
		type_eng_only_radial,
		type_eng_only_transversal,
	};
	/// двигатель направлен так, чтобы аргумент перицентра сохран€лс€ неизменным
	double eng_law_const_periarg( double anomaly)
	{
		double eccentricity = m_orbit.GetKeplerOrbitFormat().eccentricity;
		return atan(tan(anomaly) * ( 1 + 1/(1 + eccentricity * cos(anomaly) ) ) );
	}

	double eng_law_only_radial(double anomaly)
	{
		return 0;
	}

	double eng_law_only_transversal(double anomaly)
	{
		return PiConst / 2;
	}

	typedef double (CDefaultSatellite::*t_eng_law)(double anomaly);
	map< int, t_eng_law > engine_laws;

public :
	CDefaultSatellite(const CEllepticalOrbit& _orbit):m_mass(500.0), m_engine_acc(2.0), m_orbit(_orbit)
	{
		//t_eng_law law = eng_law_const_periarg;
		engine_laws[0] = eng_law_const_periarg;
		engine_laws[1] = eng_law_only_radial;
		engine_laws[2] = eng_law_only_transversal;
	}

	double GetMass() { return m_mass;}
	/// получить орбиту спутника
	const CEllepticalOrbit & GetSatelliteOrbit() {return m_orbit;}

	/// получить компоненты ускорени€ двигател€ в заданной точке орбиты
	AccelerationComponent GetAccelerationComponent(double true_anomaly)
	{
		t_eng_law law = engine_laws[m_control_law];
		//(*law)(true_anomaly);
	}


	/// сменить закон управлени€ двигателем
	/// параметр law по сути представл€ет номер закона управлени€,
	/// который определ€етс€ пользователем класса
	void SwitchControlLaw(int law)
	{
		m_control_law = law;
	}


	int GetControlLaw() const
	{
		return m_control_law;
	}



};

#endif // !SATELLITE_H

