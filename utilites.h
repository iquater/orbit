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


	/** ��������� ������� dif_ - ������������ �������� ������ */
	double dif_ecc(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_focal(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_periapsis_arg(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_periapsis(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_apsis(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	double dif_tetta(double anomaly, double engine_direction, double engine_force, kepler_orbit* orbit);

	/** �������� ��������� */
	double dif_ecc_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_focal_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_pericenter_ang_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_tetta_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	double dif_sigma_proj(double anomaly, double acc_r, double acc_n, kepler_orbit* orbit);

	/**
	 * @brief ���������� �������� ��� ��������� ������ ���������
	 * �������������� � ����������
	 */
	double velocity_raise_apo(kepler_orbit* current_orbit, double new_apocenter, const double gravy = EarthGravy); /// �/�

	/**
	 * @brief ���������� �������� ��� ��������� ������ ����������
	 * �������������� � ���������
	 */
	double velocity_raise_peri(kepler_orbit* current_orbit, double new_pericenter, const double gravy = EarthGravy);


	typedef double(*Elements_f)(double, double, double, CEllepticalOrbit *);

	void DebugPrintOrbit( CEllepticalOrbit&);
}



#endif // !UTILITES_H
