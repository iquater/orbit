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
		maneuver_passive				= 0,	/// пассивный участок
		maneuver_complanar_apo_raise	= 1,	/// компланарный маневр, повысить апоцентр
		maneuver_complanar_apo_descend	= 2,	/// компланарный маневр, понизить апоцентр
		maneuver_complanar_peri_raise	= 3,	/// компланарный маневр, повысить перицентр
		maneuver_complanar_peri_descend	= 4,	/// компланарный маневр, понизить перицентр

		maneuver_complanar_peri_arg		= 5,	/// компланарный маневр, изменение положения линии апсид
	};

	struct SPath
	{
		virtual maneuver_type GetType() = 0;
		~SPath(){}
	};


	/**
	 * @brief структура для расчета продолжительности пассивного движения;
	 * При создании объекта через кастомный конструктор,
	 * происходит расчет времени пассивного движения спутника;
	 * В дальнейшем можно добавить запись положения спутника в каждую единицу времени
	 */
	struct CPassivePath: public SPath
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

		maneuver_type GetType() { return maneuver_passive;}
	private:
		CPassivePath();/// запрет на создание пустых обЪектов
	};

	/**
	 * @brief базовая структура для расчета активного участка полета (с включенными двигателями)
	 * Пока не очень ясно как решать вопрос с направлением тяги двигателя;
	 * На данный момента от этой структуры  наследуются остальные структуры,
	 * в которых описывается активный участок с соответствующим законом направления вектора тяги
	 */
	struct CActivePath: public SPath
	{
		CEllepticalOrbit orbit;			/// начальная орбита
		double begin_true_anomaly;		/// начальное положение, [рад]
		double step;					/// шаг интегрирования, [c]
		double engine_time;				/// продолжительность работы двигателя, [с]
		double engine_acc;				/// ускарение создаваемое двигателем [м/(с*с)]
		history_t history;				/// результаты моделирования
		maneuver_type type;				/// тип маневра
		double duration;				/// продолжительность маневра

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
	 *  @brief активный учатсок плоского маневра с постоянным аргументом перицентра орбиты;
	 * направление тяги получено, исходя из того, что  в системе оскулирующих элементов
	 * производная от аргумента перицентра по времени равна нулю, т.е. аргумент перицентра = const
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

		const kepler_orbit & get_finish_pos();

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
		maneuver_type type;
		double delta_v; // необходимое приращение скорости
	public:

		CSetApocenter(double apocenter, const CEllepticalOrbit& orbit);

		/** @brief получить траекторию трансфера спутника (активный участок должен начинаться в окрестности перицентра)
		 *  @param[in] начальная истинная аномалия 
		 *  @param[in] время работы двигателя
		 *  @param[in] ускорение создаваемое двигателем
		 *	@param[in] шаг, [c]
		 */
		std::vector<SPath *> GetTransferTrajectory(double begin_true_anomaly, double engine_time, double engine_acc, double step);

	};

	class CSetPericenter: public IOperation
	{
		CEllepticalOrbit current_orbit;
		double new_pericenter; // km
		maneuver_type type;
	public:
		/**
		*/
		CSetPericenter(double periapcenter, const CEllepticalOrbit& _current_orbit);

		std::vector<SPath *> GetTransferTrajectory(double begin_true_anomaly, double engine_time, double engine_acc, double step);
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