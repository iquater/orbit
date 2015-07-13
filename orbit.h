#ifndef ORBIT_H
#define ORBIT_H

const double Gravy_const = 6.67408 * (10 ^ (-11));

class CPlanet
{
	const double m_planet_mass; /// ����� �������
	const double m_radius;		///������ �������
public:
	CPlanet(double planet_mass, ///����� �������
		double radius			///������ �������
		);

	const double & GetPlanetMass() { return m_planet_mass; }

	const double & GetPlanetRadius(){ return m_radius; }
};

class COrbit
{
private:
	CPlanet m_planet;
	double m_semi_major_axis;	/// ������� �������
	double m_eccentricity;		/// �������������� ������
	double m_periapsis;			/// �������� ������
	double m_inclination;		/// ���������� ������
	double m_ascending_nod;		/// ������� ����������� ����
	double m_mean_anomaly;		/// ������� ��������
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