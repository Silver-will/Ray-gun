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
	if (!ray_t.Surrounds(root))
	{
		root = (-half_b + sqrtd) / a;
		if (!ray_t.Surrounds(root))
			return false;
	}
	
	hit.t = root;
	hit.p = r.At(hit.t);
	glm::vec3 outwardNormal = (hit.p - center) / radius;
	hit.SetFaceNormal(r, outwardNormal);
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
	auto phi = atan2(-p.z, p.x) + pi;
	u = phi / (2 * pi);
	v = theta / pi;
}

Translate::Translate(std::shared_ptr<Shape> s, const glm::vec3& displacement)
	: object{s},offset{displacement} 
{
}

bool Translate::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)
{
	Ray offset_r(r.GetOrigin() - offset, r.GetDirection(), r.GetTime());

	if (!object->RayHit(offset_r, hit, ray_t))
		return false;

	hit.p += offset;

	return true;
}

AABB Translate::GetBoundingBox()const
{
	return box;
}


RotateY::RotateY(std::shared_ptr<Shape> p, double angle) : 
	object{p}
{
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	box = object->GetBoundingBox();

	Point min(infinity, infinity, infinity);
	Point max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = i * box.x.max + (1 - i) * box.x.min;
				auto y = j * box.y.max + (1 - j) * box.y.min;
				auto z = k * box.z.max + (1 - k) * box.z.min;

				auto newx = cos_theta * x + sin_theta * z;
				auto newz = -sin_theta * x + cos_theta * z;

				glm::vec3 tester(newx, y, newz);

				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	box = AABB(min, max);
}

bool RotateY::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)
{
	auto origin = r.GetOrigin();
	auto direction = r.GetDirection();

	origin[0] = cos_theta * r.GetOrigin()[0] - sin_theta * r.GetOrigin()[2];
	origin[2] = sin_theta * r.GetOrigin()[0] + cos_theta * r.GetOrigin()[2];

	direction[0] = cos_theta * r.GetDirection()[0] - sin_theta * r.GetDirection()[2];
	direction[2] = sin_theta * r.GetDirection()[0] + cos_theta * r.GetDirection()[2];

	Ray rotated_r(origin, direction, r.GetTime());

	// Determine whether an intersection exists in object space (and if so, where)
	if (!object->RayHit(rotated_r, hit, ray_t))
		return false;

	// Change the intersection point from object space to world space
	auto p = hit.p;
	p[0] = cos_theta * hit.p[0] + sin_theta * hit.p[2];
	p[2] = -sin_theta * hit.p[0] + cos_theta * hit.p[2];

	// Change the normal from object space to world space
	auto normal = hit.normal;
	normal[0] = cos_theta * hit.normal[0] + sin_theta * hit.normal[2];
	normal[2] = -sin_theta * hit.normal[0] + cos_theta * hit.normal[2];

	hit.p = p;
	hit.normal = normal;

	return true;
}

AABB RotateY::GetBoundingBox()const
{
	return box;
}