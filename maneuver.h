#ifndef MANEUVER_H
#define MANEUVER_H
#include "orbit.h"

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