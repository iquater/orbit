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

	/**
	 * @brief ��������� ��� ������� ����������������� ���������� ��������;
	 * ��� �������� ������� ����� ��������� �����������,
	 * ���������� ������ ������� ���������� �������� ��������;
	 * � ���������� ����� �������� ������ ��������� �������� � ������ ������� �������
	 */
	struct CPassivePath
	{
		CEllepticalOrbit orbit;		/// ��������� ������ ���������� �������
		double begin_true_anomaly;	/// ��������� ����� ��������, [���]
		double end_true_anomaly;	/// �������� ������ ��������, [���]
		double step;				/// ��� �������������, [c]
		double duration;			/// ����������������� ��������, [�]

		CPassivePath(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _end_true_anomaly,
			double _step);
	private:
		CPassivePath();/// ������ �� �������� ������ ��������
	};

	/**
	 * @brief ������� ��������� ��� ������� ��������� ������� ������ (� ����������� �����������)
	 * ���� �� ����� ���� ��� ������ ������ � ������������ ���� ���������;
	 * �� ������ ������� �� ���� ���������  ����������� ��������� ���������,
	 * � ������� ����������� �������� ������� � ��������������� ������� ����������� ������� ����
	 */
	struct CActivePath
	{
		CEllepticalOrbit orbit;		/// ��������� ������
		double begin_true_anomaly;	/// ��������� ���������, [���]
		double step;				/// ��� ��������������, [c]
		double engine_time;			/// ����������������� ������ ���������, [�]
		double engine_acc;		/// ��������� ����������� ���������� [�/(�*�)]
		history_t history;			/// ���������� �������������

		CActivePath(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_acc);
		~CActivePath(){}
	};

	/**
	 *  @brief �������� ������� �������� ������� � ���������� ���������� ���������� ������;
	 * ����������� ���� ��������, ������ �� ����, ���  � ������� ������������ ���������
	 * ����������� �� ��������� ���������� �� ������� ����� ����, �.�. �������� ���������� = const
	 */
	struct CActivePlanConstArg: public CActivePath
	{
		CActivePlanConstArg(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_acc);

	protected:
		/** 
		 * @brief ���������� ���� ����� ������ �������� � ������������ ����
		 * @param[in] eccentricity
		 * @param[in] true_anomaly, [���]
		 * @return ���� ������������ ����
		 */
		double _engine_angle( double eccentricity, double true_anomaly)
		{
			return atan(tan(true_anomaly) * ( 1 + 1/(1 + eccentricity * cos(true_anomaly) ) ) );
		}

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
		double delte_v; // ����������� ���������� ��������
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