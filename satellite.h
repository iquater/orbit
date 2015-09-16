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
	///����������� ����������
	virtual ~ISatellite(){}

	/// ������� ����� ��������
	virtual double GetMass() = 0; // ��

	/// �������� ������ ��������
	virtual const CEllepticalOrbit & GetSatelliteOrbit() = 0;

	/// �������� ���������� (���������������, ����������) ��������� ��������� � �������� ����� ������
	virtual AccelerationComponent GetAccelerationComponent(double true_anomaly) = 0;

	/// ������� ����� ���������� ����������
	/// �������� law �� ���� ������������ ����� ������ ����������,
	/// ������� ������������ ������������� ������
	virtual void SwitchControlLaw(int law) = 0;

	virtual int GetControlLaw() const = 0 ;

	/// ���������� ����� ��������� ������ ��������
	virtual void SetOrbit(const kepler_orbit & orbit) = 0;
	virtual void SetOrbit(const CEllepticalOrbit & orbit) = 0;
};

class CDefaultSatellite: public ISatellite
{
	double m_mass; // ��
	double m_engine_acc; // �/�^2
	CEllepticalOrbit m_orbit;
	int m_control_law;

	typedef double (t_eng_law)(double anomaly);

	map< int, t_eng_law > engine_laws;

	/// ��������� ��������� ���, ����� �������� ���������� ���������� ����������
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

public :
	CDefaultSatellite(const CEllepticalOrbit& _orbit):m_mass(500.0), m_engine_acc(2.0), m_orbit(_orbit)
	{

	}

	double GetMass() { return m_mass;}
	/// �������� ������ ��������
	const CEllepticalOrbit & GetSatelliteOrbit() {return m_orbit;}

	/// �������� ���������� ��������� ��������� � �������� ����� ������
	AccelerationComponent GetAccelerationComponent(double true_anomaly)
	{

	}


	/// ������� ����� ���������� ����������
	/// �������� law �� ���� ������������ ����� ������ ����������,
	/// ������� ������������ ������������� ������
	void SwitchControlLaw(int law);

	int GetControlLaw() const;


};

#endif // !SATELLITE_H

