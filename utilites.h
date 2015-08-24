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
	 * �������������� ������� �������� � ���������������
	 * �������� http://murison.alpheratz.net/dynamics/twobody/KeplerIterations_summary.pdf
	 * ������ �� python ( � ����� ����������) https://github.com/RazerM/orbital
	 */
	double ConvertEccentricFromMean(double mean_anomaly, double eccentricity, double tolerance);

	double ConvertEccentricFromTrue(double true_anomaly, double eccentricity);

	double ConvertMeanFromEccentric(double eccentric_anomaly, double eccentricity);

	double ConvertMeanFromTrue(double true_anomaly, double eccentricity);

	double ConvertTrueFromEccentric(double eccentric_anomaly, double eccentricity);

	double ConvertTrueFromMean(double mean_anomaly, double eccentricity);

	void DebugPrintOrbit( CEllepticalOrbit&);
}



#endif // !UTILITES_H
