#ifndef COLOR_H
#define COLOR_H
#include<glm/glm.hpp>
#include<iostream>
#include<span>
#include "Shape.h"
#include"Material.h"
#include"Common.h"



void AnyHit(ShapeContainer& shapes, Ray& r, HitRecord& hit, const double& tMin, const double& tMax);
bool WorldHit(const Ray& r, ShapeContainer& shapes, const Interval& ray_t, HitRecord& rec);
inline void WriteColor(std::ofstream& out, const Color& col, const int samples)
{
	float scale = 1.0 / samples;
	auto r = col.x;
	auto g = col.y;
	auto b = col.z;
	
	r *= scale;
	g *= scale;
	b *= scale;

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	static const Interval intensity(0.000, 0.999);
	out << static_cast<int>(255.999 * intensity.Clamp(r)) << ' '
		<< static_cast<int>(255.999 * intensity.Clamp(g)) << ' '
		<< static_cast<int>(255.999 * intensity.Clamp(b)) << "\n";
}

inline Color RayColor(const Ray& r, ShapeContainer& shapes, int max_depth) 
{
	HitRecord rec;
	static Color background(0.7,0.8,1.0);
	if (max_depth <= 0)
		return Color(0.0f);
	if (!WorldHit(r, shapes, Interval(0.001, Common::infinity), rec))
		return background;
	Ray scattered;
	Color attenuation;
	Color FromEmission = rec.mat->Emitted(rec.u, rec.v, rec.p);
	if (!rec.mat->Scatter(r, rec, attenuation, scattered))
	{
		return FromEmission;
	}

	Color ColorFromScatter = attenuation * RayColor(scattered, shapes, max_depth - 1);
	
	return ColorFromScatter + FromEmission;
}

#endif

