#ifndef COLOR_H
#define COLOR_H
#include<glm/glm.hpp>
#include<iostream>
#include<span>
#include "Shape.h"

inline void WriteColor(std::ofstream& out, const Color& col, const int samples)
{
	auto r = col.x;
	auto g = col.y;
	auto b = col.z;
	float scale = 1.0 / samples;
	r *= scale;
	g *= scale;
	b *= scale;

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	static const Interval intensity(0.000, 0.999);
	out << static_cast<int>(255.999 * intensity.clamp(r)) << ' '
		<< static_cast<int>(255.999 * intensity.clamp(g)) << ' '
		<< static_cast<int>(255.999 * intensity.clamp(b)) << "\n";
}
Color RayColor(const Ray& r, ShapeContainer& shapes,const Interval& ray_t, int max_depth);
void AnyHit(ShapeContainer& shapes, Ray& r, HitRecord& hit, const double& tMin, const double& tMax);
#endif

