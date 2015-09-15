#include "maneuver_scheme.h"

namespace utilites
{
	maneuver_scheme::maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit):init_orbit(_init_orbit), target_orbit(_target_orbit)
	{
		if (_init_orbit.eccentricity == 0)
		{
			if(_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				assert (_target_orbit.pericenter <= target_orbit.apocenter);
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				from_elleptic_pericenter_change(transfer_orbits.at(0).finish_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				from_elleptic_pericenter_change(transfer_orbits.at(0).finish_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				assert (_target_orbit.pericenter <= target_orbit.apocenter);
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				from_elleptic_pericenter_change(transfer_orbits.at(0).finish_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter == _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if (_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter == _target_orbit.apocenter)
			{
				from_elleptic_pericenter_change(_init_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
		}
		else if (_target_orbit.eccentricity == 0)
		{
			if(_init_orbit.pericenter > _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				from_elleptic_pericenter_change(transfer_orbits.at(0).finish_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				from_elleptic_pericenter_change(transfer_orbits.at(0).finish_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter == _target_orbit.pericenter && _init_orbit.apocenter > _target_orbit.apocenter)
			{
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter < _target_orbit.apocenter)
			{
				from_elleptic_apocenter_change(_init_orbit, _target_orbit, transfer_orbits);
				from_elleptic_pericenter_change(transfer_orbits.at(0).finish_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
			else if(_init_orbit.pericenter < _target_orbit.pericenter && _init_orbit.apocenter == _target_orbit.apocenter)
			{
				from_elleptic_pericenter_change(_init_orbit, _target_orbit, transfer_orbits);
				type = virtual_maneuver_unknown;
			}
		}
		else 
		{

		}

		/*if( init_orbit.eccentricity == 0 && init_orbit.pericenter ==  target_orbit.pericenter && init_orbit.apocenter < target_orbit.apocenter)
		{
			from_circle_to_elleptic_r(init_orbit, target_orbit, transfer_orbits);
			type = virtual_maneuver_circle_to_tangent_elleptic_raise;
		}

		else if( init_orbit.eccentricity == 0 && init_orbit.apocenter ==  target_orbit.apocenter && init_orbit.pericenter > target_orbit.pericenter)
		{
			from_circle_to_elleptic_d(init_orbit, target_orbit, transfer_orbits);
			type = virtual_maneuver_circle_to_tangent_elleptic_descend;
		}

		else if(init_orbit.eccentricity == 0 &&  target_orbit.eccentricity == 0 && init_orbit.pericenter < target_orbit.pericenter)
		{
			from_circle_to_circle_raise_hohmann(_init_orbit, _target_orbit, transfer_orbits);
			type = virtual_maneuver_circle_to_circle_raise_hohmann;
		}

		else if(init_orbit.eccentricity == 0 &&  target_orbit.eccentricity == 0 && init_orbit.pericenter > target_orbit.pericenter)
		{
			from_circle_to_circle_descend_hohmann(_init_orbit, _target_orbit, transfer_orbits);
			type = virtual_maneuver_circle_to_circle_descend_hohmann;
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
		assert(orbit.apocenter >= orbit.pericenter);

		double result = sqrt ( 2 * EarthGravy /( orbit.pericenter + orbit.apocenter) * orbit.apocenter / orbit.pericenter );
		return result ;
	}

	double maneuver_scheme::apocenter_velocity(const kepler_orbit& orbit)
	{
		assert(orbit.apocenter);
		assert(orbit.pericenter);
		assert(orbit.apocenter >= orbit.pericenter);

		double result = sqrt ( 2 * EarthGravy /( orbit.pericenter + orbit.apocenter) * orbit.pericenter / orbit.apocenter );
		return result ;
	}

	/// с круговой на внешнюю эллиптическую орбиту
	void maneuver_scheme::from_circle_to_elleptic_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity == 0);
		assert(_init_orbit.pericenter <=  _target_orbit.pericenter);
		assert(_init_orbit.apocenter < _target_orbit.apocenter);
		assert(_target_orbit.pericenter_angle >= 0 );

		double vc = circle_velocity(_init_orbit); 
		double vp = pericenter_velocity(_target_orbit);

		double impulse = vp - vc;

			
		virtual_transfer_orbit vto;
		vto.init_orbit = _init_orbit;
		vto.finish_orbit = _target_orbit;
		vto.impulse = impulse;
		vto.type = virtual_maneuver_circle_to_tangent_elleptic_raise;
		vto.imp_true_anomaly = _target_orbit.pericenter_angle;

		_transfer_orbits.push_back(vto);
	}

	/// с круговой на внутренюю эллиптическую орбиту
	void maneuver_scheme::from_circle_to_elleptic_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity == 0);
		assert(_init_orbit.apocenter == _target_orbit.apocenter);
		assert(_init_orbit.pericenter > _target_orbit.pericenter);
		assert(_target_orbit.pericenter_angle >= 0 );

		double vc = circle_velocity(_init_orbit); 
		double va = apocenter_velocity(_target_orbit);

		double impulse = vc - va;

		virtual_transfer_orbit vto;
		vto.init_orbit = _init_orbit;
		vto.finish_orbit = _target_orbit;
		vto.impulse = (-1)*impulse; // для понижения орбиты импульс направлен в сторону, противоположную движению
		vto.imp_true_anomaly = (_target_orbit.pericenter_angle < PiConst) ? _target_orbit.pericenter_angle + PiConst : _target_orbit.pericenter_angle - PiConst;
		vto.type = virtual_maneuver_circle_to_tangent_elleptic_descend;

		_transfer_orbits.push_back(vto);
	}

	void maneuver_scheme::from_elleptic_to_circle_r(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity != 0);
		assert(_init_orbit.apocenter == _target_orbit.apocenter);
		assert(_init_orbit.pericenter < _target_orbit.pericenter);
		assert(_target_orbit.pericenter_angle >= 0 );

		double va = apocenter_velocity(_init_orbit);
		double vc = circle_velocity(_target_orbit); 

		double impulse = vc - va;

		virtual_transfer_orbit vto;
		vto.init_orbit = _init_orbit;
		vto.finish_orbit = _target_orbit;
		vto.impulse = impulse;
		vto.type = virtual_maneuver_elleptic_to_tangent_circle_raise;
		vto.imp_true_anomaly = PiConst; // прикладываем импульс в апоцентре

		_transfer_orbits.push_back(vto);
	}

	void maneuver_scheme::from_elleptic_to_circle_d(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity != 0);
		assert(_init_orbit.pericenter == _target_orbit.pericenter);
		assert(_init_orbit.apocenter > _target_orbit.apocenter);
		assert(_target_orbit.pericenter_angle >= 0 );

		double vp = pericenter_velocity(_init_orbit);
		double vc = circle_velocity(_target_orbit); 

		double impulse = vc - vp;

		virtual_transfer_orbit vto;
		vto.init_orbit = _init_orbit;
		vto.finish_orbit = _target_orbit;
		vto.impulse = impulse;
		vto.type = virtual_maneuver_elleptic_to_tangent_circle_descend;
		vto.imp_true_anomaly = 0; // прикладываем импульс в перицентре

		_transfer_orbits.push_back(vto);
	}

	void maneuver_scheme::from_circle_to_circle_raise_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity == 0 && _target_orbit.eccentricity == 0);
		assert(_init_orbit.apocenter < _target_orbit.apocenter && _init_orbit.pericenter < _target_orbit.pericenter);
		assert(_target_orbit.pericenter_angle >= 0 );

		kepler_orbit elleptic_transfer;// эллипс Гомана

		elleptic_transfer.pericenter = _init_orbit.pericenter;
		elleptic_transfer.apocenter = _target_orbit.apocenter;
		elleptic_transfer.eccentricity = calc_eccentricity(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
		elleptic_transfer.focal = calc_focal(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
		elleptic_transfer.pericenter_angle = 0;
			
		from_circle_to_elleptic_r(_init_orbit, elleptic_transfer, _transfer_orbits);
		assert(_transfer_orbits.size());
		from_elleptic_to_circle_r(elleptic_transfer, _target_orbit, _transfer_orbits);

	}

	void maneuver_scheme::from_circle_to_circle_descend_hohmann(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity == 0 && _target_orbit.eccentricity == 0);
		assert(_init_orbit.apocenter > _target_orbit.apocenter && _init_orbit.pericenter > _target_orbit.pericenter);
		assert(_target_orbit.pericenter_angle >= 0 );

		kepler_orbit elleptic_transfer;// эллипс Гомана

		elleptic_transfer.pericenter = _target_orbit.pericenter;
		elleptic_transfer.apocenter = _init_orbit.apocenter;
		elleptic_transfer.eccentricity = calc_eccentricity(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
		elleptic_transfer.focal = calc_focal(elleptic_transfer.pericenter, elleptic_transfer.apocenter);
		elleptic_transfer.pericenter_angle = 0;
			
		from_circle_to_elleptic_d(_init_orbit, elleptic_transfer, _transfer_orbits);
		assert(_transfer_orbits.size());
		from_elleptic_to_circle_d(elleptic_transfer, _target_orbit, _transfer_orbits);

	}

	void maneuver_scheme::from_elleptic_apocenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		//assert( _target_orbit.apocenter > _init_orbit.pericenter);
		//assert( _targer_orbit.pericenter < _init_orbit.apocenter);

		double init_vel_p = pericenter_velocity(_init_orbit); // скорость в перицентре начальной орбиты

		kepler_orbit transfer;
		transfer.pericenter =( _init_orbit.pericenter > _target_orbit.apocenter ? _target_orbit.apocenter : _init_orbit.pericenter);
		transfer.apocenter = ( _init_orbit.pericenter > _target_orbit.apocenter ?_init_orbit.pericenter: _target_orbit.apocenter );
		transfer.eccentricity = calc_eccentricity(transfer.pericenter, transfer.apocenter);
		transfer.focal = calc_focal(transfer.pericenter, transfer.apocenter);
		transfer.pericenter_angle = _init_orbit.pericenter_angle;

		double transfer_vel_p = pericenter_velocity(transfer);

		double impulse = transfer_vel_p - init_vel_p;

		virtual_transfer_orbit vto;
		vto.init_orbit = _init_orbit;
		vto.finish_orbit = transfer;
		vto.impulse = (_init_orbit.apocenter < _target_orbit.apocenter) ?impulse : (-1)*impulse;
		vto.type =(_init_orbit.apocenter < _target_orbit.apocenter)? virtual_maneuver_elleptic_apocenter_raise : virtual_maneuver_elleptic_apocenter_descend;
		vto.imp_true_anomaly = 0; // прикладываем импульс в перицентре

		_transfer_orbits.push_back(vto);
	}

	void maneuver_scheme::from_elleptic_pericenter_change(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		//assert( _target_orbit.apocenter > _init_orbit.pericenter);
		//assert( _targer_orbit.pericenter < _init_orbit.apocenter);

		double init_vel_p = apocenter_velocity(_init_orbit); // скорость в перицентре начальной орбиты

		kepler_orbit transfer;
		transfer.pericenter = ( _target_orbit.pericenter > _init_orbit.apocenter ?_init_orbit.apocenter: _target_orbit.pericenter);
		transfer.apocenter = ( _target_orbit.pericenter > _init_orbit.apocenter ? _target_orbit.pericenter: _init_orbit.apocenter);
		transfer.eccentricity = calc_eccentricity(transfer.pericenter, transfer.apocenter);
		transfer.focal = calc_focal(transfer.pericenter, transfer.apocenter);
		transfer.pericenter_angle = _init_orbit.pericenter_angle;

		double transfer_vel_p = apocenter_velocity(transfer);

		double impulse =  transfer_vel_p - init_vel_p;

		virtual_transfer_orbit vto;
		vto.init_orbit = _init_orbit;
		vto.finish_orbit = transfer;
		vto.impulse = (_init_orbit.pericenter < _target_orbit.pericenter) ?impulse : (-1)*impulse;
		vto.type =(_init_orbit.pericenter < _target_orbit.pericenter)? virtual_maneuver_elleptic_pericenter_raise : virtual_maneuver_elleptic_pericenter_descend;
		vto.imp_true_anomaly = PiConst; // прикладываем импульс в перицентре

		_transfer_orbits.push_back(vto);
	}

	//с эллептической на эллиптическую по окружности
	void maneuver_scheme::from_elleptic_to_elleptic_circle_transfer(const kepler_orbit & _init_orbit, const kepler_orbit & _target_orbit, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		if ( _init_orbit == _target_orbit) return;

		if (_init_orbit.apocenter > _target_orbit.apocenter)
		{
			from_elleptic_apocenter_change(_init_orbit, _target_orbit, _transfer_orbits);
			kepler_orbit temp = _transfer_orbits.at( _transfer_orbits.size() -1).finish_orbit;

			kepler_orbit circle_temp = make_circle(temp.apocenter);
				
			from_elleptic_to_circle_d(temp, circle_temp, _transfer_orbits);

			temp = _transfer_orbits.at( _transfer_orbits.size() -1).finish_orbit;
			from_circle_to_elleptic_d(temp, _target_orbit, _transfer_orbits);
		}
		else if (_init_orbit.apocenter < _target_orbit.apocenter)
		{
			from_elleptic_apocenter_change(_init_orbit, _target_orbit, _transfer_orbits);
			kepler_orbit temp = _transfer_orbits.at( _transfer_orbits.size() -1).finish_orbit;

			kepler_orbit circle_temp = make_circle(temp.apocenter);
			from_elleptic_to_circle_r(temp, circle_temp, _transfer_orbits);

			temp = _transfer_orbits.at( _transfer_orbits.size() -1).finish_orbit;
			from_circle_to_elleptic_r(temp, _target_orbit, _transfer_orbits);
		}

	}

	void maneuver_scheme::change_apocenter(const kepler_orbit & _init_orbit, double new_apocenter, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		kepler_orbit new_orbit;
		new_orbit.apocenter = new_apocenter;
		new_orbit.pericenter = _init_orbit.pericenter;
		new_orbit.eccentricity = calc_eccentricity(new_orbit);
		new_orbit.focal = calc_focal(new_orbit);
		new_orbit.pericenter_angle = _init_orbit.pericenter_angle;

		from_elleptic_apocenter_change(_init_orbit, new_orbit, _transfer_orbits);
		
	}


	void maneuver_scheme::change_pericenter(const kepler_orbit & _init_orbit, double new_pericneter, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		kepler_orbit new_orbit;
		new_orbit.apocenter = _init_orbit.apocenter;
		new_orbit.pericenter =new_pericneter;
		new_orbit.eccentricity = calc_eccentricity(new_orbit);
		new_orbit.focal = calc_focal(new_orbit);
		new_orbit.pericenter_angle = _init_orbit.pericenter_angle;

		from_elleptic_pericenter_change(_init_orbit, new_orbit, _transfer_orbits);
	}

	void maneuver_scheme::change_pericenter_argument(const kepler_orbit & _init_orbit, double new_pericneter_ang, std::vector<virtual_transfer_orbit>& _transfer_orbits)
	{
		assert(_init_orbit.eccentricity !=0 ); // для круговой орбиты не имеет смысла изменять аргумент перигея - он всегда нулевой
		assert ( new_pericneter_ang >= 0 && new_pericneter_ang <= PiConst);

		kepler_orbit circle = make_circle(circle.apocenter); // переходная орбита

		from_elleptic_to_circle_r(_init_orbit, circle, _transfer_orbits);

		kepler_orbit target = _init_orbit;
		target.pericenter_angle = new_pericneter_ang;

		from_circle_to_elleptic_d(circle, target, _transfer_orbits);
	}

}
