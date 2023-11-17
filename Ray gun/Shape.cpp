#include "Shape.h"
#include "Material.h"
#include<iostream>
Sphere::Sphere(Point orig, float rad, Color col) : center{orig},
	radius{rad}, color{col},isMoving{false}
{

}

Sphere::Sphere(Point orig, float rad, std::shared_ptr<Material> Mat) : center{ orig },
radius{ rad }, mat{Mat}, color{glm::vec3(0.0f)}, isMoving{false}
{
	auto rvec = Point(radius);
	BBox = AABB(orig - rvec, orig + rvec);

}

Sphere::Sphere(Point orig, Point orig1, float rad, std::shared_ptr<Material> Mat) : center{ orig },
 radius {rad}, mat{ Mat }, color{ glm::vec3(0.0f) }, isMoving{ true }
{
	centerVec = orig1 - orig;
	auto rvec = Point(radius);
	AABB aabb(orig - rvec, orig + rvec);
	AABB aabb1(orig1 - rvec, orig1 + rvec);
	BBox = AABB(aabb, aabb1);
}

bool Sphere::RayHit(const Ray& r, HitRecord& hit,const Interval& ray_t)
{
	auto center = LerpCenter(r.GetTime());
	glm::vec3 oc = r.GetOrigin() - center;
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
	glm::vec3 outwardNormal = (hit.p - center) / radius;
	hit.setFaceNormal(r, outwardNormal);
	GetSphereUV(outwardNormal, hit.u, hit.v);
	hit.mat = mat;

	return true;
}

Color Sphere::GetColor()
{
	return color;
}

Point Sphere::GetPos()
{
	return center;
}

Point Sphere::LerpCenter(double time)
{
	if (isMoving)
	{
		return center + (float)time * centerVec;
	}
	else return center;
}

AABB Sphere::GetBoundingBox()const
{
	return BBox;
}

void Sphere::GetSphereUV(const Point& p, double& u, double& v)
{
	auto theta = acos(-p.y);
	auto phi = atan2(-p.z, p.x + pi);
	u = phi / (2 * pi);
	v = theta / pi;
}