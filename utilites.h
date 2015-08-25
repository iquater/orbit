#ifndef UTILITES_H
#define UTILITES_H
#include "constants.h"
#include <assert.h>
#include "orbit.h"
namespace utilites
{
	const size_t MAX_ITERATIONS = 100;


	/**
	 * @brief ConvertMeanToEccentric
	 * преобразование средней аномалии в эксцентрическую
	 * источник http://murison.alpheratz.net/dynamics/twobody/KeplerIterations_summary.pdf
	 * версия на python ( и автор реализации) https://github.com/RazerM/orbital
	 */
	double ConvertEccentricFromMean(double mean_anomaly, double eccentricity, double tolerance);

	double ConvertEccentricFromTrue(double true_anomaly, double eccentricity);

	double ConvertMeanFromEccentric(double eccentric_anomaly, double eccentricity);

	double ConvertMeanFromTrue(double true_anomaly, double eccentricity);

	double ConvertTrueFromEccentric(double eccentric_anomaly, double eccentricity);

	double ConvertTrueFromMean(double mean_anomaly, double eccentricity);


	/** семейство функций dif_ - оскулирующие элементы орбиты */
	double dif_ecc(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit);

	double dif_focal(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit);

	double dif_periapsis_arg(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit);

	double dif_periapsis(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit);

	double dif_apsis(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit);

	double dif_tetta(double anomaly, double engine_direction, double engine_force, CEllepticalOrbit* orbit);

	typedef double(*Elements_f)(double, double, double, CEllepticalOrbit *);

	/*Elements_f dif_motion[] = //система дифференциальных уравнений
	{ 
		dif_ecc, 
		dif_focal, 
		dif_periapsis_arg, 
		dif_periapsis, 
		dif_apsis, 
		dif_tetta 
	};*/

	void DebugPrintOrbit( CEllepticalOrbit&);
}



#endif // !UTILITES_H
