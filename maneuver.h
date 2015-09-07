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
		maneuver_passive				= 0,	/// ��������� �������
		maneuver_complanar_apo_raise	= 1,	/// ������������ ������, �������� ��������
		maneuver_complanar_apo_descend	= 2,	/// ������������ ������, �������� ��������
		maneuver_complanar_peri_raise	= 3,	/// ������������ ������, �������� ���������
		maneuver_complanar_peri_descend	= 4,	/// ������������ ������, �������� ���������

		maneuver_complanar_peri_arg		= 5,	/// ������������ ������, ��������� ��������� ����� �����
	};

	struct SPath
	{
		virtual maneuver_type GetType() = 0;
		~SPath(){}
	};


	/**
	 * @brief ��������� ��� ������� ����������������� ���������� ��������;
	 * ��� �������� ������� ����� ��������� �����������,
	 * ���������� ������ ������� ���������� �������� ��������;
	 * � ���������� ����� �������� ������ ��������� �������� � ������ ������� �������
	 */
	struct CPassivePath: public SPath
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

		maneuver_type GetType() { return maneuver_passive;}
	private:
		CPassivePath();/// ������ �� �������� ������ ��������
	};

	/**
	 * @brief ������� ��������� ��� ������� ��������� ������� ������ (� ����������� �����������)
	 * ���� �� ����� ���� ��� ������ ������ � ������������ ���� ���������;
	 * �� ������ ������� �� ���� ���������  ����������� ��������� ���������,
	 * � ������� ����������� �������� ������� � ��������������� ������� ����������� ������� ����
	 */
	struct CActivePath: public SPath
	{
		CEllepticalOrbit orbit;			/// ��������� ������
		double begin_true_anomaly;		/// ��������� ���������, [���]
		double step;					/// ��� ��������������, [c]
		double engine_time;				/// ����������������� ������ ���������, [�]
		double engine_acc;				/// ��������� ����������� ���������� [�/(�*�)]
		history_t history;				/// ���������� �������������
		maneuver_type type;				/// ��� �������

		CActivePath(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_acc,
			maneuver_type _type);
		~CActivePath(){}

		maneuver_type GetType(){ return type;}
	};

	/**
	 *  @brief �������� ������� �������� ������� � ���������� ���������� ���������� ������;
	 * ����������� ���� ��������, ������ �� ����, ���  � ������� ������������ ���������
	 * ����������� �� ��������� ���������� �� ������� ����� ����, �.�. �������� ���������� = const
	 * 
	 */
	struct CActivePlanConstArg: public CActivePath
	{
		CActivePlanConstArg(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_acc,
			maneuver_type type,
			double constraint);

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
		maneuver_type type;
		double delte_v; // ����������� ���������� ��������
	public:

		CSetApocenter(double apocenter, const CEllepticalOrbit& orbit);

		std::vector<SPath *> GetTransferTrajectory();

	};

	class CSetPericenter: public IOperation
	{
		CEllepticalOrbit current_orbit;
		double new_pericenter; // km
	public:
		/**
		*/
		CSetPericenter(double periapcenter, const CEllepticalOrbit& _current_orbit);

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