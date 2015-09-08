#ifndef CONSTANTS_H
#define CONSTANTS_H

#define _USE_MATH_DEFINES
#include <math.h>

///гравитационная постоянная:
const double Gravy_const = 6.67408 * (pow(10.0, -11));

///число Pi:
const double PiConst = M_PI;

///масса Земли в кг:
const double EarthMass = 5.97219 * (pow(10.0, 24));

///гравитационная постоянная Земли
const double EarthGravy = Gravy_const * EarthMass;


#endif // !CONSTANTS_H
