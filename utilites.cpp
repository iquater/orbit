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

}