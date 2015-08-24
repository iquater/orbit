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

	void DebugPrintOrbit( CEllepticalOrbit& orbit)
	{
		qDebug() << "Orbit Paramaters:";
		qDebug() << QString("semi-major axis %1").arg( orbit.GetSemiMajorAxis() );
		qDebug() << QString("Eccentricity %1").arg(orbit.GetEccentricity());
		qDebug() << QString("Periapsis %1").arg(orbit.GetPeriapsis());
		qDebug() << QString("Apsis %1").arg(orbit.GetApsis());
	}
}