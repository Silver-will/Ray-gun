#ifndef MATERIAL_H
#define MATERIAL_H
#include "Common.h"
#include "Shape.h"
#include "Texture.h"
struct HitRecord;
struct Ray;

struct Material 
{
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec,Color& attenuation, Ray& scattered)const = 0;
	virtual Color Emitted(double u, double v, const Point& p);
};

struct Lambertian : public Material {
	Lambertian(const Color& a);
	Lambertian(std::shared_ptr<Texture> a);
	bool Scatter(const Ray& r_in, const HitRecord& rec,Color& attenuation, Ray& scattered)const override;

private:
	std::shared_ptr<Texture> albedo;
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

struct DiffuseLight : public Material {
	DiffuseLight(std::shared_ptr<Texture> a);
	DiffuseLight(Color c);
	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const override;
	Color Emitted(double u, double v, const Point& p);
private:
	std::shared_ptr<Texture> emit;
};
#endif // !MATERIAL_H

