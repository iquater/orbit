#ifndef MANEUVER_H
#define MANEUVER_H
#include "orbit.h"


enum maneuver_cases
{
	maneuver_complanar_increase_periapsis = 1,
	maneuver_complanar_increase_apsis = 2,
	maneuver_complanar_increase_apsis_and_periapsis = 3,
};
class CProfile
{

};

class CManeuver
{
	CEllepticalOrbit* initial_orbit;
	CEllepticalOrbit* target_orbit;

	CManeuver(){}
	CManeuver(const CManeuver&){}
	
	bool isComplanar();
public:
	CManeuver(CEllepticalOrbit* _initial_orbit,
		CEllepticalOrbit* _target_orbit);

	void getTrajectory();

	
};
#endif // !MANEUVER_H