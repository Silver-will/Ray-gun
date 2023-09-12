#ifndef COMMON_H
#define COMMON_H

#include<cmath>
#include<limits>
#include<memory>
#include<cstdlib>
#include<glm/glm.hpp>

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

inline glm::vec3 RandomVector()
{
	return glm::vec3(random_double(), random_double(), random_double());
}

inline glm::vec3 RandomVector(double min, double max)
{
	return glm::vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

inline glm::vec3 RandomInUnitSphere()
{
	while (true)
	{
		auto p = RandomVector(-1, 1);
		if (glm::dot(p, p) < 1)
		{
			return p;
		}
	}
}

inline glm::vec3 RandomUnitVector()
{
	return glm::normalize(RandomInUnitSphere());
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal)
{
	auto on_unit_sphere = RandomUnitVector();
	if (glm::dot(on_unit_sphere, normal) > 0.0)
	{
		return on_unit_sphere;
	}
	else
	{
		return -on_unit_sphere;
	}
}

inline double LinearToGamma(double linear_value)
{
	return pow(linear_value, 1.0 / 2.0);
}
#endif