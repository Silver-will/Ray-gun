#ifndef COMMON_H
#define COMMON_H

#include<cmath>
#include<limits>
#include<memory>
#include<cstdlib>
#include"Typedefs.h"
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

inline int random_int(int min, int max)
{
	return static_cast<int>(random_double(min, max));
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

inline bool IsVectorNearZero(glm::vec3& v)
{
	auto s = 1e-8;
	return ((fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s));
}

inline glm::vec3 Reflect(const glm::vec3& v,const glm::vec3& n)
{
	return v - 2 * glm::dot(v, n) * n;
}

inline glm::vec3 Refract(const glm::vec3& uv, const glm::vec3& n, float etai_over_etat)
{
	auto cos_theta = fmin(glm::dot(-uv, n), 1.0f);
	glm::vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	glm::vec3 r_out_parallel = -sqrt(fabs(1.0f - glm::dot(r_out_perp, r_out_perp))) * n;
	return r_out_perp + r_out_parallel;
}

inline glm::vec3 RandomInUnitDisk()
{
	while (true)
	{
		auto p = glm::vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (glm::dot(p, p) < 1)
		{
			return p;
		}
	}
}
#endif