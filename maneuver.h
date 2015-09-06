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

	struct CPassivePath
	{
		double begin_true_anomaly;
		CEllepticalOrbit orbit;
		double duration;
	};

	struct CActivePath
	{
		history_t history;
		double duration;
	};

	class IOperation
	{
	public:
		virtual ~IOperation(){};
	};

	class CSetApocenter: public IOperation
	{
		CEllepticalOrbit current_orbit;
		double new_apocenter; // km
		double delte_v; // необходимое приращение скорости
	public:

		CSetApocenter(double apocenter, const CEllepticalOrbit& orbit);
		void Simulate();
	};

	class CSetPericenter: public IOperation
	{
		CEllepticalOrbit current_orbit;
		double new_pericenter; // km
	public:
		/**
		*/
		CSetPericenter(double periapcenter, const CEllepticalOrbit& _current_orbit);
		
		void Simulate();
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