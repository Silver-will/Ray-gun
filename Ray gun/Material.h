#ifndef MATERIAL_H
#define MATERIAL_H
#include "Common.h"
#include "Shape.h"
#include "Texture.h"
#include "PDF.h"
struct HitRecord;
struct Ray;

struct ScatterRecord {
	Color attenuation;
	std::shared_ptr<PDF> pdf_ptr;
	bool skip_pdf;
	Ray skip_pdf_ray;
};

struct Material 
{
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec)const {
		return false;
	}
	virtual Color Emitted(const Ray& r_in, const HitRecord& rec,double u, double v, const Point& p);
	virtual double scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const {
		return 0;
	}
};

struct Lambertian : public Material {
	Lambertian(const Color& a);
	Lambertian(std::shared_ptr<Texture> a);
	bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec)const override;
	double scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const;

private:
	std::shared_ptr<Texture> albedo;
};

struct Metal : public Material {
	Metal(const Color& a, float f);
	bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec)const override;

private:
	Color albedo;
	float fuzz;
};

struct Dielectric : public Material {
	Dielectric(float index_of_refraction);
	bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec)const override;
private:
	static double reflectance(double cosine, double ref_idx);
	double ir;
};

struct DiffuseLight : public Material {
	DiffuseLight(std::shared_ptr<Texture> a);
	DiffuseLight(Color c);
	//bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec)const override;
	Color Emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const Point& p) override;
private:
	std::shared_ptr<Texture> emit;
};

struct Isotropic : public Material {
	Isotropic(Color c);
	Isotropic(std::shared_ptr<Texture> a);
	bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec)const override;
	double scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const override;

private:
	std::shared_ptr<Texture> albedo;
};
#endif // !MATERIAL_H

