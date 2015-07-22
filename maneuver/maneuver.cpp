#include "maneuver.h"

CManeuver::CManeuver(CEllepticalOrbit* _initial, CEllepticalOrbit* _target):
initial_orbit(_initial), 
target_orbit(_target)
{

}


bool CManeuver::isComplanar()
{
	return false;
}


void CManeuver::getTrajectory()
{
	double a1 = initial_orbit->GetApsis(), a2 = target_orbit->GetApsis();
	double p1 = initial_orbit->GetPeriapsis(), p2 = target_orbit->GetPeriapsis();
	double e1 = initial_orbit->GetEccentricity(), e2 = target_orbit->GetEccentricity();
	double i1 = initial_orbit->GetInclination(), i2 = target_orbit->GetInclination();
	double n1 = initial_orbit->GetAscendingNode(), n2 = target_orbit->GetAscendingNode();
}