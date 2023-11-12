#include "Material.h"
#include "Hit.h"
#include<iostream>
Lambertian::Lambertian(const Color& a) : albedo{a}
{

}

bool Lambertian::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)const
{
	auto scatter_dir = rec.normal + RandomUnitVector();
	if (IsVectorNearZero(scatter_dir))
	{
		std::cout << "z" << std::endl;
		scatter_dir = rec.normal;
	}
	scattered = Ray(rec.p, scatter_dir,r_in.GetTime());
	attenuation = albedo;
	return true;
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
	float sin_theta = pow((1.0 - cos_theta*cos_theta), 1.0 / 2.0);
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
