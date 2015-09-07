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
	 * @brief структура для расчета продолжительности пассивного движения;
	 * При создании объекта через кастомный конструктор,
	 * происходит расчет времени пассивного движения спутника;
	 * В дальнейшем можно добавить запись положения спутника в каждую единицу времени
	 */
	struct CPassivePath
	{
		CEllepticalOrbit orbit;		/// параметры орбиты пассивного участка
		double begin_true_anomaly;	/// начальная точка движения, [рад]
		double end_true_anomaly;	/// конечная точыка движения, [рад]
		double step;				/// шаг моделирования, [c]
		double duration;			/// продолжительность движения, [с]

		CPassivePath(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _end_true_anomaly,
			double _step);
	private:
		CPassivePath();/// запрет на создание пустых обЪектов
	};

	/**
	 * @brief базовая структура для расчета активного участка полета (с включенными двигателями)
	 * Пока не очень ясно как решать вопрос с направлением тяги двигателя;
	 * На данный момента от этой структуры  наследуются остальные структуры,
	 * в которых описывается активный участок с соответствующим законом направления вектора тяги
	 */
	struct CActivePath
	{
		CEllepticalOrbit orbit;		/// начальная орбита
		double begin_true_anomaly;	/// начальное положение, [рад]
		double step;				/// шаг интегрирования, [c]
		double engine_time;			/// продолжительность работы двигателя, [с]
		double engine_acc;		/// ускарение создаваемое двигателем [м/(с*с)]
		history_t history;			/// результаты моделирования

		CActivePath(CEllepticalOrbit _orbit, 
			double _begin_true_anomaly,
			double _step,
			double _engine_time,
			double _engine_acc);
		~CActivePath(){}
	};

	/**
	 *  @brief активный учатсок плоского маневра с постоянным аргументом перицентра орбиты;
	 * направление тяги получено, исходя из того, что  в системе оскулирующих элементов
	 * производная от аргумента перицентра по времени равна нулю, т.е. аргумент перицентра = const
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
		 * @brief возвращает угол между радиус вектором и направлением тяги
		 * @param[in] eccentricity
		 * @param[in] true_anomaly, [рад]
		 * @return угол навправления тяги
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