#include "maneuver_scheme.h"

namespace utilites
{
	maneuver_scheme::maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit):init_orbit(_init_orbit), target_orbit(_target_orbit)
	{
		if (_init_orbit.eccentricity == 0)_init_orbit.pericenter_angle = 0;
		if(_target_orbit.eccentricity == 0) _target_orbit.pericenter_angle = 0;


		if ( _init_orbit.pericenter_angle == _target_orbit.pericenter_angle)
		{
				assert (_target_orbit.pericenter <= _target_orbit.apocenter);
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);
				
				kepler_orbit temp_orb;
				if(transfer_orbits.size()) temp_orb = transfer_orbits.at(0).finish_orbit;
				else temp_orb = _init_orbit;

				((temp_orb.pericenter_angle - _init_orbit.pericenter_angle != PiConst)? change_pericenter : change_apocenter )(temp_orb, _target_orbit.pericenter, transfer_orbits);
		}
		else if (fabs (_init_orbit.pericenter_angle - _target_orbit.pericenter_angle ) == PiConst) // euj
		{
				assert (_target_orbit.pericenter <= _target_orbit.apocenter);
				change_pericenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);
				
				kepler_orbit temp_orb;
				if(transfer_orbits.size()) temp_orb = transfer_orbits.at(0).finish_orbit;
				else temp_orb = _init_orbit;

				((temp_orb.pericenter_angle - _init_orbit.pericenter_angle != PiConst)? change_apocenter :  change_pericenter )(temp_orb, _target_orbit.pericenter, transfer_orbits);
		}
		else if( _init_orbit.pericenter_angle != _target_orbit.pericenter_angle)
		{
			change_pericenter_argument(_init_orbit, _target_orbit.pericenter_angle, transfer_orbits);
		}

		/*	if (false && _init_orbit.eccentricity == 0 && target_orbit.pericenter_angle == _init_orbit.pericenter_angle) // с внешней круговой на внутреннюю произвольную
		{

			if(_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				assert (_target_orbit.pericenter <= _target_orbit.apocenter);
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);

				const kepler_orbit & temp_orb = transfer_orbits.at(0).finish_orbit;

				((temp_orb.pericenter_angle - _init_orbit.pericenter_angle != PiConst)? change_pericenter : change_apocenter )(temp_orb, _target_orbit.pericenter, transfer_orbits);

				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);

				const kepler_orbit & temp_orb = transfer_orbits.at(0).finish_orbit;

				change_pericenter(temp_orb, _target_orbit.pericenter, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				assert (_target_orbit.pericenter <= _target_orbit.apocenter);
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);

				const kepler_orbit & temp_orb = transfer_orbits.at(0).finish_orbit;

				change_pericenter(temp_orb, _target_orbit.pericenter, transfer_orbits);

				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter == _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter == _target_orbit.apocenter)
			{
				change_pericenter(_init_orbit, _target_orbit.pericenter, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
		}
		else if (false && _target_orbit.eccentricity == 0 && target_orbit.pericenter_angle == _init_orbit.pericenter_angle) // с внешней произвольной на внутреннюю круговую
		{
			if(_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				assert (_target_orbit.pericenter <= _target_orbit.apocenter);
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);

				const kepler_orbit & temp_orb = transfer_orbits.at(0).finish_orbit;
				((temp_orb.pericenter_angle - _init_orbit.pericenter_angle != PiConst)? change_pericenter : change_apocenter )(temp_orb, _target_orbit.pericenter, transfer_orbits);

				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);

				const kepler_orbit & temp_orb = transfer_orbits.at(0).finish_orbit;

				change_pericenter(temp_orb, _target_orbit.pericenter, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);
				const kepler_orbit & temp_orb = transfer_orbits.at(0).finish_orbit;

				change_pericenter(temp_orb, _target_orbit.pericenter, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter == _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				change_apocenter(_init_orbit, _target_orbit.apocenter, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter == _target_orbit.apocenter)
			{
				change_pericenter(_init_orbit, _target_orbit.pericenter, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
		}*/
	}
	
	double maneuver_scheme::circle_velocity(const kepler_orbit& orbit)
	{
		assert(orbit.apocenter == orbit.pericenter);
		assert(orbit.apocenter!=0);

		double result = sqrt ( EarthGravy / orbit.apocenter );
		return result;
	}
	double maneuver_scheme::pericenter_velocity(const kepler_orbit& orbit)
	{
		assert(orbit.apocenter);
		assert(orbit.pericenter);
	//	assert(orbit.apocenter >= orbit.pericenter);

		double result = sqrt ( 2 * EarthGravy /( orbit.pericenter + orbit.apocenter) * orbit.apocenter / orbit.pericenter );
		return result ;
	}

	double maneuver_scheme::apocenter_velocity(const kepler_orbit& orbit)
	{
		assert(orbit.apocenter);
		assert(orbit.pericenter);
	//	assert(orbit.apocenter >= orbit.pericenter);

		double result = sqrt ( 2 * EarthGravy /( orbit.pericenter + orbit.apocenter) * orbit.pericenter / orbit.apocenter );
		return result ;
	}

	void maneuver_scheme::change_apocenter(const kepler_orbit & _init_orbit, double new_apocenter, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		//assert(new_apocenter >= _init_orbit.pericenter);
		if (_init_orbit.apocenter == new_apocenter) return;
		kepler_orbit new_orbit;
		new_orbit.apocenter = (new_apocenter < _init_orbit.pericenter)? _init_orbit.pericenter: new_apocenter;
		new_orbit.pericenter = (new_apocenter < _init_orbit.pericenter)? new_apocenter: _init_orbit.pericenter;
		new_orbit.eccentricity = calc_eccentricity(new_orbit);
		new_orbit.focal = calc_focal(new_orbit);
		new_orbit.pericenter_angle = (new_apocenter < _init_orbit.pericenter)? _init_orbit.pericenter_angle + PiConst : _init_orbit.pericenter_angle;

		if(!new_orbit.eccentricity)
			new_orbit.pericenter_angle = 0;

		if (new_orbit.pericenter_angle >= 2 * PiConst)
			new_orbit.pericenter_angle = new_orbit.pericenter_angle - 2 * PiConst;

		double init_vel_p = pericenter_velocity(_init_orbit);
		double target_vel_p =  (new_apocenter< _init_orbit.pericenter)? apocenter_velocity(new_orbit): pericenter_velocity(new_orbit);
		double impulse = target_vel_p - init_vel_p;

		virtual_transfer_orbit vto;
		vto.finish_orbit = new_orbit;
		vto.transversal_impulse = impulse;
		vto.radial_impulse = 0;
		vto.imp_true_anomaly = 0;
		vto.init_orbit = _init_orbit;
		vto.type = (impulse > 0) ? virtual_maneuver_elleptic_apocenter_raise : virtual_maneuver_elleptic_apocenter_descend;
		_transfer_orbits.push_back(vto);
	}

	void maneuver_scheme::change_pericenter(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		//assert(_init_orbit.apocenter >= new_pericneter);
		if (_init_orbit.pericenter == new_pericneter) return;
		kepler_orbit new_orbit;
		new_orbit.apocenter = (new_pericneter > _init_orbit.apocenter)? new_pericneter: _init_orbit.apocenter;
		new_orbit.pericenter = (new_pericneter > _init_orbit.apocenter)? _init_orbit.apocenter :new_pericneter;
		new_orbit.eccentricity = calc_eccentricity(new_orbit);
		new_orbit.focal = calc_focal(new_orbit);
		new_orbit.pericenter_angle = (new_pericneter > _init_orbit.apocenter)? _init_orbit.pericenter_angle + PiConst : _init_orbit.pericenter_angle ;

		if(!new_orbit.eccentricity)
			new_orbit.pericenter_angle = 0;

		if (new_orbit.pericenter_angle >= 2 * PiConst)
			new_orbit.pericenter_angle = new_orbit.pericenter_angle - 2 * PiConst;

		double init_vel_a = apocenter_velocity(_init_orbit);
		double target_vel_a =(new_pericneter > _init_orbit.apocenter)?pericenter_velocity(new_orbit): apocenter_velocity(new_orbit);
		double impulse = target_vel_a - init_vel_a;

		virtual_transfer_orbit vto;
		vto.finish_orbit = new_orbit;
		vto.transversal_impulse = impulse;
		vto.radial_impulse = 0;
		vto.imp_true_anomaly = PiConst;
		vto.init_orbit = _init_orbit;
		vto.type = (impulse > 0) ? virtual_maneuver_elleptic_pericenter_raise : virtual_maneuver_elleptic_pericenter_descend;
		_transfer_orbits.push_back(vto);
	}

	void maneuver_scheme::change_pericenter_argument(const kepler_orbit & _init_orbit, double new_pericneter_ang, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity !=0 ); // для круговой орбиты не имеет смысла изменять аргумент перигея - он всегда нулевой
		assert ( new_pericneter_ang >= 0 && new_pericneter_ang <= 2 * PiConst);

		double delta_arg = new_pericneter_ang - _init_orbit.pericenter_angle;

		double impulse = -2 * sqrt (EarthGravy / _init_orbit.focal) * _init_orbit.eccentricity * sin(delta_arg / 2);
		double true_anomaly = delta_arg / 2;
		virtual_transfer_orbit vto;
		vto.init_orbit = _init_orbit;
		vto.finish_orbit = _init_orbit; vto.finish_orbit.pericenter_angle = new_pericneter_ang;

		vto.imp_true_anomaly = true_anomaly;
		vto.radial_impulse = impulse;
		vto.transversal_impulse = 0;
		vto.type = virtual_maneuver_pericenter_arg_changed;

		_transfer_orbits.push_back(vto);
	}

}
