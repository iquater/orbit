#include "utilites.h"
#include <qdebug.h>
#include <qstring.h>
using namespace utilites;

namespace utilites
{
	double ConvertEccentricFromMean(double M, double e, double tolerance)
	{
		double E = 0;
		double MNorm = fmod(M, 2 * PiConst);

		double E0 = M + (-1 / 2 * pow(e, 3) + e + (pow(e, 2) + 3 / 2 * cos(M) * pow(e, 3)) * cos(M)) * sin(M);

		double dE = tolerance + 1;

		size_t count = 0;
		while (dE > tolerance)
		{
			double t1 = cos(E0), t2 = -1 + e * t1, t3 = sin(E0), t4 = e * t3,
				t5 = -E0 + t4 + MNorm, t6 = t5 / (0.5 * t5 * t4 / t2 + t2);
			double E = E0 - t5 / ((0.5 * t3 - 1 / 6 * t1 * t6) * e * t6 + t2);

			dE = abs(E - E0);
			E0 = E;

			assert(count > MAX_ITERATIONS);
			++count;
		}

		return E;
	}

	double ConvertEccentricFromTrue(double ta, double e)
	{
		double E = atan2(sqrt(1 - pow(e, 2)) * sin(ta), e + cos(ta));
		E = fmod(E, 2 * PiConst);
		return E >= 0 ? E : E + 2 * PiConst;
	}

	double ConvertMeanFromEccentric(double ea, double e)
	{
		return ea - e * sin(ea);
	}

	double ConvertMeanFromTrue(double ta, double e)
	{
		double ea = ConvertEccentricFromTrue(ta, e);
		return ConvertMeanFromEccentric(ea, e);
	}

	double ConvertTrueFromEccentric(double ea, double e)
	{
		return 2 * atan2(sqrt(1 + e) * sin(ea / 2), sqrt(1 - e) * cos(ea / 2));
	}

	double ConvertTrueFromMean(double ma, double e)
	{
		double ea = ConvertEccentricFromMean(ma, e, 0.0001);
		return ConvertTrueFromEccentric(ea, e);
	}

	/*void DebugPrintOrbit( CEllepticalOrbit& orbit)
	{
		qDebug() << "Orbit Paramaters:";
		qDebug() << QString("semi-major axis %1").arg( orbit.GetSemiMajorAxis() );
		qDebug() << QString("Eccentricity %1").arg(orbit.GetEccentricity());
		qDebug() << QString("Periapsis %1").arg(orbit.GetPeriapsis());
		qDebug() << QString("Apsis %1").arg(orbit.GetApsis());
	}*/

	double dif_ecc(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit)
	{
		double result = 0;
		/// de/dt:
		double a = engine_force * sqrt(orbit->focal / EarthGravy);
		double b = sin(anomaly)* cos(engine_direction);
		double c = orbit->eccentricity * cos(anomaly) * cos(anomaly) + 2 * cos(anomaly) + orbit->eccentricity;
		double d = 1 + orbit->eccentricity*cos(anomaly);
		double e = sin(engine_direction);

		result = a * (b + c * e / d);

		return result;
	}

	double dif_focal(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit)
	{
		double result = 0;
		/// dp/dt:

		double a = 2 * engine_force * orbit->focal;
		double b = 1 + orbit->eccentricity * cos(anomaly);
		double c = sqrt(orbit->focal / EarthGravy) * sin(engine_direction);

		result = a * c / b;

		return result;
	}

	double dif_periapsis_arg(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit)
	{
		double result = 0;
		/// dw/dt:	
		double a =0, b = 0, c = 0, d = 0;
		a = engine_force / orbit->eccentricity * sqrt(orbit->focal / EarthGravy);
		b = cos(anomaly) * cos(engine_direction);
		c = (2 + orbit->eccentricity * cos(anomaly)) * sin(anomaly);
		d = sin(engine_direction) / (1 + orbit->eccentricity * cos(anomaly));

		result = a * (-b + c * d);

		return result;
	}

	double dif_periapsis(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit)
	{
		double result = 0;
		/// dperiapsis/dt:	
		double a = engine_force * orbit->focal  * sqrt(orbit->focal / EarthGravy);
		double b = 1 + orbit->eccentricity * orbit->eccentricity;
		double c = sin(anomaly) * cos(engine_direction);
		double d = sin(engine_direction) / (1 + orbit->eccentricity * cos(anomaly));
		double e = 2 * (1 - cos(anomaly)) + orbit->eccentricity * sin(anomaly) * sin(anomaly);

		result = a / b *(-c - e * d);

		return result;
	}

	double dif_apsis(double anomaly, double engine_direction, double engine_force, kepler_orbit * orbit)
	{
		double result = 0;

		double a = engine_force * orbit->focal  * sqrt(orbit->focal / EarthGravy);
		double b = 1 - orbit->eccentricity * orbit->eccentricity;
		double c = sin(anomaly) * cos(engine_direction);
		double d = sin(engine_direction) / (1 + orbit->eccentricity * cos(anomaly));
		double e = 2 * (1 + cos(anomaly)) - orbit->eccentricity * sin(anomaly) * sin(anomaly);

		result = a / b *(c - e * d);

		return result;
	}
	double dif_tetta(double anomaly, double engine_direction, double engine_force, kepler_orbit * orbit)
	{
		double result = 0;
		double r = (orbit->focal / (1 + orbit->eccentricity * cos(anomaly)));
		double a = sqrt(EarthGravy * orbit->focal) /
			(r * r);

		double b = engine_force * cos(anomaly) / orbit->eccentricity;
		double c = sqrt(orbit->focal / EarthGravy) * cos(engine_direction);
		double d = engine_direction * sin(anomaly) / orbit->eccentricity * (1 + r / orbit->focal);
		double f = sqrt(orbit->focal / EarthGravy) * sin(engine_direction);

		result = a + b * c - d * f;

		return result;
	}

	double velocity_raise_apo(kepler_orbit* orb, double new_apocenter, const double gravy )
	{
		new_apocenter = new_apocenter - orb->apocenter;
		double result = sqrt (2 * EarthGravy / orb->pericenter) * 
			( sqrt( (orb->apocenter + new_apocenter) / ( orb->apocenter + orb->pericenter + new_apocenter) ) -
			sqrt (orb->apocenter / (orb->pericenter + orb->apocenter) ) );

		return result;
	}

	double velocity_raise_peri(kepler_orbit* orb, double new_pericenter, const double gravy )
	{
		new_pericenter = new_pericenter - orb->pericenter;

		double result = sqrt (2 * EarthGravy / orb->apocenter) * 
			( sqrt( (orb->pericenter + new_pericenter) / (orb->apocenter + orb->pericenter + new_pericenter)) -
			sqrt(orb->pericenter / (orb->pericenter + orb->apocenter)));

		return result;
	}


	double dif_ecc_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit)
	{
		double result;

		double r = orbit->focal / (1 + orbit->eccentricity * cos(anomaly));

		double a = sqrt(orbit->focal / EarthGravy);
		double b = acc_r * sin(anomaly) + acc_n  * orbit->eccentricity * r / orbit->focal +
			acc_n * (1 + r / orbit->focal)*cos(anomaly);

		result = a * b;

		return result;
	
	}

	double dif_focal_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit)
	{
		double result;

		double r = orbit->focal / (1 + orbit->eccentricity * cos(anomaly));

		result = 2 * acc_n * r * sqrt(orbit->focal / EarthGravy);

		return result;
	}

	double dif_pericenter_ang_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit)
	{
		double result;

		if (orbit->eccentricity == 0)
			return 0;

		double r = orbit->focal / (1 + orbit->eccentricity * cos(anomaly));

		double a = sqrt(orbit->focal / EarthGravy) / orbit->eccentricity;

		double b = -acc_r * cos(anomaly) + acc_n * (1 + r / orbit->focal) * sin(anomaly); // здесь не хватает части по наклонению

		result = a * b;

		return result;
	}

	double dif_tetta_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit)
	{
		double result;

		double r = orbit->focal / (1 + orbit->eccentricity * cos(anomaly));

		result = sqrt(EarthGravy * orbit->focal) / (r*r) - dif_sigma_proj(anomaly, acc_r, acc_n, orbit);

		return result;
	}

	double dif_sigma_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit)
	{
		double result;
		
		if (orbit->eccentricity == 0)
			return 0;

		double r = orbit->focal / (1 + orbit->eccentricity * cos(anomaly));

		double a = sqrt(orbit->focal / EarthGravy) / orbit->eccentricity;

		double b = -acc_r * cos(anomaly) + acc_n * (1 + r / orbit->focal) * sin(anomaly); // здесь не хватает части по наклонению

		result = a * b;

		return result;
	}

	double calc_eccentricity(double pericenter, double apocenter)
	{
		return (apocenter - pericenter)/ (apocenter + pericenter);
	}

	double calc_focal(double pericenter, double apocenter)
	{
		return (2 *apocenter * pericenter)/ (apocenter + pericenter);
	}
	
	double calc_eccentricity (const kepler_orbit &  orbit)
	{
		return (orbit.apocenter - orbit.pericenter)/ (orbit.apocenter + orbit.pericenter);
	}


	double calc_focal(const kepler_orbit &  orbit)
	{
		return (2 * orbit.apocenter * orbit.pericenter)/ (orbit.apocenter + orbit.pericenter);
	}


	kepler_orbit make_circle(double radius)
	{
		kepler_orbit circle;
		circle.apocenter = radius;
		circle.pericenter = radius;
		circle.eccentricity = 0;
		circle.focal = radius;

		return circle;
	}

	maneuver_scheme::maneuver_scheme(kepler_orbit & _init_orbit, kepler_orbit & _target_orbit):init_orbit(_init_orbit), target_orbit(_target_orbit)
	{
		if( init_orbit.eccentricity == 0 && init_orbit.pericenter ==  target_orbit.pericenter && init_orbit.apocenter < target_orbit.apocenter)
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
		}
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
		assert(_init_orbit.pericenter ==  _target_orbit.pericenter);
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
		transfer.pericenter = _init_orbit.pericenter;
		transfer.apocenter = _target_orbit.apocenter;
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
		transfer.pericenter = _target_orbit.pericenter;
		transfer.apocenter = _init_orbit.apocenter;
		transfer.eccentricity = calc_eccentricity(transfer.pericenter, transfer.apocenter);
		transfer.focal = calc_focal(transfer.pericenter, transfer.apocenter);
		transfer.pericenter_angle = _init_orbit.pericenter_angle;

		double transfer_vel_p = apocenter_velocity(transfer);

		double impulse =  init_vel_p - transfer_vel_p;

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
