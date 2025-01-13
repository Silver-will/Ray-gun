#include "Material.h"
#include "Hit.h"
#include "ONB.h"
#include<iostream>

Color Material::Emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const Point& p)
{
	return Color(0);
}

Lambertian::Lambertian(const Color& a) : albedo(std::make_shared<SolidColor>(a))
{

}

Lambertian::Lambertian(std::shared_ptr<Texture> a) : albedo{a}
{
	
}

bool Lambertian::Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
{
	srec.attenuation = albedo->Value(rec.u, rec.v, rec.p);
	srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
	srec.skip_pdf = false;

	/*ONB uvw;
	uvw.build_from_w(rec.normal);
	auto scatter_dir = uvw.local(RandomCosineDirection());
	//auto scatter_dir = rec.normal + RandomUnitVector();
	//auto scatter_dir = RandomOnHemisphere(rec.normal);
	if (IsVectorNearZero(scatter_dir))
	{
		scatter_dir = rec.normal;
	}
	scattered = Ray(rec.p, scatter_dir,r_in.GetTime());
	attenuation = albedo->Value(rec.u,rec.v,rec.p);
	*/
	return true;
}

double Lambertian::scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const
{
	auto cos_theta = glm::dot(rec.normal, glm::normalize(scattered.GetDirection()));
	return cos_theta < 0 ? 0 : cos_theta / pi;
	
	//return 1 / (2 * pi);
}

Metal::Metal(const Color& a, float f) : albedo{ a },fuzz(f < 1 ? f : 1 )
{

}

bool Metal::Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
{
	glm::vec3 reflected = Reflect(r_in.GetDirection(), rec.normal);
	reflected = glm::normalize(reflected) + (fuzz * RandomUnitVector());

	srec.attenuation = albedo;
	srec.pdf_ptr = nullptr;
	srec.skip_pdf = true;
	srec.skip_pdf_ray = Ray(rec.p, reflected, r_in.GetTime());
	
	return true;
}

Dielectric::Dielectric(float index_of_refraction): ir{index_of_refraction}
{
	
}

bool Dielectric::Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
{
	srec.attenuation = Color(1.0f);
	srec.pdf_ptr = nullptr;
	srec.skip_pdf = true;

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
	srec.skip_pdf_ray = Ray(rec.p, direction, r_in.GetTime());
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

Color DiffuseLight::Emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const Point& p)
{
	if (!rec.frontFace)
		return Color(0);
	return emit->Value(u, v, p);
}

Isotropic::Isotropic(Color c) : albedo{std::make_shared<SolidColor>(c)}
{

}

Isotropic::Isotropic(std::shared_ptr<Texture> a) : albedo{a}
{

}

double Isotropic::scattering_pdf(const Ray& r_in, const HitRecord& rec, Ray& scattered) const
{
	return 1 / (4 * pi);
}

bool Isotropic::Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
{
	srec.attenuation = albedo->Value(rec.u,rec.v,rec.p);
	srec.pdf_ptr = std::make_shared<SpherePDF>();
	srec.skip_pdf = false;
	return true;
}