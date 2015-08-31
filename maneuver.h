#ifndef MANEUVER_H
#define MANEUVER_H
#include "orbit.h"
#include <stdlib.h>
#include <vector>
//#include <memory>

using namespace std;


namespace maneuver
{
	enum maneuver_type
	{

	};

	//typedef shared_ptr<IOperation> Operation;

	class IOperation
	{
	public:
		virtual ~IOperation(){};
	};

	class CSetApsis: public IOperation
	{
		double m_apsis;
		double delta_v;
	public:
		CSetApsis(double apsis,const CEllepticalOrbit& orbit);
	};

	class CSetPeriapsis: public IOperation
	{
	public:
		CSetPeriapsis(double periapsis,const CEllepticalOrbit& orbit);
	};

	class CSetRound: public IOperation
	{
	public:
		CSetRound(const CEllepticalOrbit& orbit);
	};

	class CSetEccentricity: public IOperation
	{
	public:
		CSetEccentricity(double ecc,const CEllepticalOrbit& orbit);
	};

	class CSetInclination
	{
	public:
		CSetInclination(double inclination,const CEllepticalOrbit& orbit);
	};

	typedef std::vector<IOperation* > operations_t;

	struct SManeuver
	{
		operations_t operations;
	};


}

#endif // !MANEUVER_H