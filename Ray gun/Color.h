#ifndef COLOR_H
#define COLOR_H
#include<glm/glm.hpp>
#include<iostream>
#include<span>
#include "ShapeList.h"
#include"Material.h"
#include"Common.h"
#include "PDF.h"
#include<fstream>


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

inline void WriteColorOnce(std::ofstream& out, const int samples, std::vector<Color>& colors)
{
	float scale = 1.0 / samples;
	const Interval intensity(0.000, 0.999);

	for (auto& col : colors)
	{
		auto r = col.x;
		auto g = col.y;
		auto b = col.z;

		if (r != r) r = 0.0;
		if (g != g) g = 0.0;
		if (b != b) b = 0.0;

		r *= scale;
		g *= scale;
		b *= scale;

		r = LinearToGamma(r);
		g = LinearToGamma(g);
		b = LinearToGamma(b);
		out << static_cast<int>(255.999 * intensity.Clamp(r)) << ' '
			<< static_cast<int>(255.999 * intensity.Clamp(g)) << ' '
			<< static_cast<int>(255.999 * intensity.Clamp(b)) << "\n";
	}
}

inline Color RayColor(const Ray& r, ShapeList& shapes, int max_depth,const Shape& lights) 
{
	static Color background(0.0);
	if (max_depth <= 0)
		return Color(0.0f);
	
	HitRecord rec;

	if (!shapes.RayHit(r, rec, Interval(0.001, Common::infinity)))
		return background;

	ScatterRecord srec;
	
	Color FromEmission = rec.mat->Emitted(r,rec,rec.u, rec.v, rec.p);
	if (!rec.mat->Scatter(r, rec, srec))
	{
		return FromEmission;
	}

	if (srec.skip_pdf)
	{
		return srec.attenuation * RayColor(srec.skip_pdf_ray, shapes, max_depth - 1,lights);
	}

	auto light_ptr = std::make_shared<HittablePDF>(lights, rec.p);
	MixturePDF p(light_ptr, srec.pdf_ptr);

	Ray scattered = Ray(rec.p, p.Generate(), r.GetTime());
	auto pdf_value = p.Value(scattered.GetDirection());

	double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

	Color sampleColor = RayColor(scattered, shapes, max_depth - 1, lights);
	Color FromScatter = (srec.attenuation * (float)scattering_pdf * sampleColor) / (float)pdf_value;

	return FromEmission + FromScatter;
}

#endif

