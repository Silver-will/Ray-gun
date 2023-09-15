#ifndef MATERIAL_H
#define MATERIAL_H
#include "Common.h"
#include "Shape.h"
struct HitRecord;
struct Ray;

struct Material 
{
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec,Color& attenuation, Ray& scattered)const = 0;
};

struct Lambertian : public Material {
	Lambertian(const Color& a);
	bool Scatter(const Ray& r_in, const HitRecord& rec,Color& attenuation, Ray& scattered)const override;

private:
	Color albedo;
};

struct Metal : public Material {
	Metal(const Color& a, float f);
	bool Scatter(const Ray& r_in, const HitRecord& rec,Color& attenuation, Ray& scattered)const override;

private:
	Color albedo;
	float fuzz;
};

struct Dielectric : public Material {
	Dielectric(float index_of_refraction);
	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const override;
private:
	static double reflectance(double cosine, double ref_idx);
	double ir;
};

#endif // !MATERIAL_H

