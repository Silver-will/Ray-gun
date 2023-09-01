#ifndef COMMON_H
#define COMMON_H

#include<cmath>
#include<limits>
#include<memory>
#include<cstdlib>

namespace Common {
	const double infinity = std::numeric_limits<double>::infinity();
	const double pi = 3.1415926535897932385;
}

inline double degress_to_radians(double degrees)
{
	return degrees * Common::pi / 180.0;
}

inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}
#endif