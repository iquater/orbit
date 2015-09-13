#include "orbit.h"
#include "constants.h"
#include "utilites.h"
#include <math.h>
#include <vector>
#include "maneuver.h"
#include "maneuver_scheme.h"
using namespace utilites;

using namespace maneuver;

int amain ()
{
	//CEllepticalOrbit * orbit = new CEllepticalOrbit(EarthMass, 6400, 6600, 7600); 

	/**CEllepticalOrbit * low_orbit = new CEllepticalOrbit(EarthMass, 6400, 6600, 6600);

	CEllepticalOrbit * gso_orbit = new CEllepticalOrbit(EarthMass, 6400, 6400+35786, 6400+35786);

	kepler_orbit low = low_orbit->GetKeplerOrbitFormat();
	kepler_orbit gso = gso_orbit->GetKeplerOrbitFormat();*/

//	maneuver_scheme scheme(low, gso);
//	maneuver_scheme scheme_rev(gso, low);

	/*std::vector<utilites::virtual_transfer_orbit> _transfers;
	maneuver_scheme::change_apocenter(low, 6500 * 1000, _transfers);*/

	/*double a = gso_orbit->GetOrbitPeriod();

	double t = orbit->GetOrbitPeriod();
	double m = orbit->GetMeanVelocity();

	CSetPericenter * apo = new CSetPericenter(7100, *orbit);
	apo->GetTransferTrajectory(0, 7, 2.5, 0.01);*/


	/*while(getchar()!='\n')
	{;}*/
	return 0;
}


