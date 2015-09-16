#include "utilites.h"

using namespace utilites;

namespace utilites
{
	enum virtual_maneuver_type
	{
		virtual_maneuver_unknown = 0,

		virtual_maneuver_elleptic_apocenter_raise , // компланарный, поднять апоцентр
		virtual_maneuver_elleptic_apocenter_descend , // компланарный, поднять апоцентр

		virtual_maneuver_elleptic_pericenter_raise , // компланарный, поднять перицентр
		virtual_maneuver_elleptic_pericenter_descend , // компланарный, поднять перицентр

		virtual_maneuver_pericenter_arg_changed , // компланарный, поднять перицентр
	};

	struct virtual_transfer_orbit
	{
		kepler_orbit init_orbit; // орбита, на которой прикладывается импульс
		kepler_orbit finish_orbit; //орбита, после приложения импульса
		double transversal_impulse;
		double radial_impulse;
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

		maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit);

	private:
		/// скорость круговой орбиты
		static double circle_velocity(const kepler_orbit& orbit);

		/// скорость в перицентре
		static double pericenter_velocity(const kepler_orbit& orbit);

		/// скорость в апоцентре
		static double apocenter_velocity(const kepler_orbit& orbit);

	public:
		/// изменить апоцентр орбиты
		static void change_apocenter(const kepler_orbit & _init_orbit, double new_apocenter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// изменить перицентр орбиты
		static void change_pericenter(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// изменить аргумент перицентра орбиты
		static void change_pericenter_argument(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
	};

}