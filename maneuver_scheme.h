#include "utilites.h"

using namespace utilites;

namespace utilites
{
	enum virtual_maneuver_type
	{
		virtual_maneuver_unknown = 0,
		virtual_maneuver_circle_to_tangent_elleptic_raise = 1, // ������������, ���������� ������
		virtual_maneuver_circle_to_tangent_elleptic_descend = 2,// ������������, ���������� ������

		virtual_maneuver_elleptic_to_tangent_circle_raise = 3, // ������������, ���������� ������
		virtual_maneuver_elleptic_to_tangent_circle_descend = 4, // ������������, ���������� ������

		virtual_maneuver_circle_to_circle_raise_hohmann = 5,// ������������, � �������� �� ��������
		virtual_maneuver_circle_to_circle_descend_hohmann = 6,// ������������, � �������� �� ��������

		virtual_maneuver_elleptic_apocenter_raise = 7, // ������������, ������� ��������
		virtual_maneuver_elleptic_apocenter_descend = 8, // ������������, ������� ��������

		virtual_maneuver_elleptic_pericenter_raise = 9, // ������������, ������� ���������
		virtual_maneuver_elleptic_pericenter_descend = 10, // ������������, ������� ���������

		virtual_maneuver_elleptic_to_cross_elleptic = 11, // ������������, �������������� ������
	};

	struct virtual_transfer_orbit
	{
		kepler_orbit init_orbit; // ������, �� ������� �������������� �������
		kepler_orbit finish_orbit; //������, ����� ���������� ��������
		double impulse;
		double imp_true_anomaly; // ����� ���������� ��������, �� ��������� ������
		virtual_maneuver_type type;
	};


	/**
	 * @brief ��������� ��������� ���������� ���������� ������� �������
	 */
	struct maneuver_scheme
	{
		const kepler_orbit init_orbit;
		const kepler_orbit target_orbit;
		std::vector<virtual_transfer_orbit> transfer_orbits;
		virtual_maneuver_type type;

		maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit);

	private:
		/// �������� �������� ������
		static double circle_velocity(const kepler_orbit& orbit);

		/// �������� � ����������
		static double pericenter_velocity(const kepler_orbit& orbit);

		/// �������� � ���������
		static double apocenter_velocity(const kepler_orbit& orbit);

		/// � �������� �� ������� ������������� ������
		static void from_circle_to_elleptic_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		/// � �������� �� ��������� ������������� ������
		static void from_circle_to_elleptic_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// � ������������� �� ������� ��������
		static void from_elleptic_to_circle_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// � ������������� �� ���������� ��������
		static void from_elleptic_to_circle_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		/// � �������� �� ������� �������� (������)
		static void from_circle_to_circle_raise_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// � �������� �� ���������� �������� (������)
		static void from_circle_to_circle_descend_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);

		/// �������� ��� �������� �������� ������
		static void from_elleptic_apocenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// �������� ��� �������� ��������� ������
		static void from_elleptic_pericenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		//� ������������� �� ������������� �� ����������
		void from_elleptic_to_elleptic_circle_transfer(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits);
	public:
		/// �������� �������� ������
		static void change_apocenter(const kepler_orbit & _init_orbit, double new_apocenter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// �������� ��������� ������
		static void change_pericenter(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
		
		/// �������� �������� ���������� ������
		static void change_pericenter_argument(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits);
	};

}