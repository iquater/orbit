#include "utilites.h"

using namespace utilites;

namespace utilites
{
	enum virtual_maneuver_type
	{
		virtual_maneuver_unknown = 0,
		virtual_maneuver_circle_to_tangent_elleptic_raise = 1, // компланарный, касающиеся орбиты
		virtual_maneuver_circle_to_tangent_elleptic_descend = 2,// компланарный, касающиеся орбиты

		virtual_maneuver_elleptic_to_tangent_circle_raise = 3, // компланарный, касающиеся орбиты
		virtual_maneuver_elleptic_to_tangent_circle_descend = 4, // компланарный, касающиеся орбиты

		virtual_maneuver_circle_to_circle_raise_hohmann = 5,// компланарный, с круговой на круговую
		virtual_maneuver_circle_to_circle_descend_hohmann = 6,// компланарный, с круговой на круговую

		virtual_maneuver_elleptic_apocenter_raise = 7, // компланарный, поднять апоцентр
		virtual_maneuver_elleptic_apocenter_descend = 8, // компланарный, поднять апоцентр

		virtual_maneuver_elleptic_pericenter_raise = 9, // компланарный, поднять перицентр
		virtual_maneuver_elleptic_pericenter_descend = 10, // компланарный, поднять перицентр

		virtual_maneuver_elleptic_to_cross_elleptic = 11, // компланарный, пересекающиеся орбиты
		virtual_maneuver_elleptic_to_elleptic_circle = 12, // компланарный, c эллиптической на эллиптическую по круговой
	};

	struct virtual_transfer_orbit
	{
		kepler_orbit init_orbit; // орбита, на которой прикладывается импульс
		kepler_orbit finish_orbit; //орбита, после приложения импульса
		double impulse;
		double imp_true_anomaly; // точка приложения импульса, на начальной орбите
		virtual_maneuver_type type;
	};

	
	/**
	 * @brief структура описывает мгновенные импульсные ПЛОСКИЕ маневры
	 */
	struct maneuver_scheme
	{
		const kepler_orbit init_orbit;
		const kepler_orbit target_orbit;
		std::vector<virtual_transfer_orbit> transfer_orbits;
		virtual_maneuver_type type;

		maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit);

	private:
		/// скорость круговой орбиты
		static double circle_velocity(const kepler_orbit& orbit);

		/// скорость в перицентре
		static double pericenter_velocity(const kepler_orbit& orbit);

		/// скорость в апоцентре
		static double apocenter_velocity(const kepler_orbit& orbit);

		/// с круговой на внешнюю эллиптическую орбиту
		static void from_circle_to_elleptic_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		/// с круговой на внутренюю эллиптическую орбиту
		static void from_circle_to_elleptic_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// с эллептической на внешнюю круговую
		static void from_elleptic_to_circle_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// с эллептической на внутреннюю круговую
		static void from_elleptic_to_circle_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		/// с круговой на внешнюю круговую (Гоманн)
		static void from_circle_to_circle_raise_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// с круговой на внутреннюю круговую (Гоманн)
		static void from_circle_to_circle_descend_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		/// повысить или понизить апоцентр орбиты
		static void from_elleptic_apocenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// повысить или понизить перицентр орбиты
		static void from_elleptic_pericenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		//с эллептической на эллиптическую по окружности
		void from_elleptic_to_elleptic_circle_transfer(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
	public:
		/// изменить апоцентр орбиты
		static void change_apocenter(const kepler_orbit & _init_orbit, double new_apocenter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// изменить перицентр орбиты
		static void change_pericenter(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// изменить аргумент перицентра орбиты
		static void change_pericenter_argument(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
	};

}