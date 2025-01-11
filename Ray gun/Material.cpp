#include "Material.h"
#include "Hit.h"
#include<iostream>

double Material::ScatteringPDF(const Ray& r_in, const HitRecord& rec, const Ray& scattered)const
{
	return 0;
}

Color Material::Emitted(double u, double v, const Point& p)
{
	return Color(0);
}

Lambertian::Lambertian(const Color& a) : albedo(std::make_shared<SolidColor>(a))
{

}

Lambertian::Lambertian(std::shared_ptr<Texture> a) : albedo{a}
{
	
}

bool Lambertian::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const
{
	//auto scatter_dir = rec.normal + RandomUnitVector();
	auto scatter_dir = RandomOnHemisphere(rec.normal);
	if (IsVectorNearZero(scatter_dir))
	{
		scatter_dir = rec.normal;
	}
	scattered = Ray(rec.p, scatter_dir,r_in.GetTime());
	attenuation = albedo->Value(rec.u,rec.v,rec.p);
	return true;
}

double Lambertian::scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const
{
	/*auto cos_theta = glm::dot(rec.normal, glm::normalize(scattered.GetDirection()));
	return cos_theta < 0 ? 0 : cos_theta / pi;
	*/
	return 1 / (2 * pi);
}

Metal::Metal(const Color& a, float f) : albedo{ a },fuzz(f < 1 ? f : 1 )
{

}

bool Metal::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const
{
	glm::vec3 reflected = Reflect(glm::normalize(r_in.GetDirection()), rec.normal);
	scattered = Ray(rec.p, reflected + fuzz * RandomUnitVector(),r_in.GetTime());
	attenuation = albedo;
	return (glm::dot(scattered.GetDirection(), rec.normal) > 0);
}

Dielectric::Dielectric(float index_of_refraction): ir{index_of_refraction}
{
	
}

bool Dielectric::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const
{
	attenuation = Color(1.0f);
	float refraction_ratio = rec.frontFace ? (1.0f / ir) : ir;
	glm::vec3 unit_direction = glm::normalize(r_in.GetDirection());
	float cos_theta = fmin(glm::dot(-unit_direction, rec.normal), 1.0);
	float sin_theta = sqrtf(1.0 - cos_theta*cos_theta);
	bool cannot_refract = refraction_ratio * sin_theta > 1.0;
	glm::vec3 direction;
	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		direction = Reflect(unit_direction, rec.normal);
	else
		direction = Refract(unit_direction, rec.normal, refraction_ratio);
	scattered = Ray(rec.p, direction, r_in.GetTime());
	return true;
}

double Dielectric::reflectance(double cosine, double ref_idx)
{
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

DiffuseLight::DiffuseLight(std::shared_ptr<Texture> a) : emit{a}
{

}

DiffuseLight::DiffuseLight(Color c) : emit{std::make_shared<SolidColor>(c)}
{

}

bool DiffuseLight::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const
{
	return false;
}

Color DiffuseLight::Emitted(double u, double v, const Point& p)
{
	return emit->Value(u, v, p);
}

Isotropic::Isotropic(Color c) : albedo{std::make_shared<SolidColor>(c)}
{

}

Isotropic::Isotropic(std::shared_ptr<Texture> a) : albedo{a}
{

}

bool Isotropic::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const
{
	scattered = Ray(rec.p, RandomUnitVector(), r_in.GetTime());
	attenuation = albedo->Value(rec.u, rec.v, rec.p);
	return true;
}