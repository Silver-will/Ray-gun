#include "Shape.h"
#include "Material.h"
#include<iostream>
Sphere::Sphere(Point orig, float rad, Color col) : Center{orig},
	radius{rad}, color{col}
{

}

Sphere::Sphere(Point orig, float rad, std::shared_ptr<Material> Mat) : Center{ orig },
radius{ rad }, mat{Mat}, color{glm::vec3(0.0f)}
{

}

Sphere::Sphere() : Center{glm::vec3(0.0f)}, radius{0.3f},
	color{glm::vec3(0.0f)}
{

}
bool Sphere::RayHit(const Ray& r, HitRecord& hit,const Interval& ray_t)
{
	glm::vec3 oc = r.GetOrigin() - Center;
	auto a = glm::dot(r.GetDirection(), r.GetDirection());
	auto half_b = glm::dot(oc, r.GetDirection());
	auto c = glm::dot(oc, oc) - radius * radius;
	auto discriminant = (half_b * half_b) - ( a * c);
	
	if (discriminant < 0.0f)
		return false;

	auto sqrtd = sqrt(discriminant);

	auto root = (-half_b - sqrtd) / a;
	if (!ray_t.surrounds(root))
	{
		root = (-half_b + sqrtd) / a;
		if (!ray_t.surrounds(root))
			return false;
	}
	
	hit.t = root;
	hit.p = r.At(hit.t);
	glm::vec3 outwardNormal = (hit.p - Center) / radius;
	hit.setFaceNormal(r, outwardNormal);
	hit.mat = mat;

	return true;
}

Color Sphere::GetColor()
{
	return color;
}

Point Sphere::GetPos()
{
	return Center;
}