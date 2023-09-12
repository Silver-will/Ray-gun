#include<fstream>
#include<iostream>

#include "Color.h"
#include"Common.h"

void AnyHit(ShapeContainer& shapes, Ray& r, HitRecord& hit, double& tMin, double& tMax)
{
	HitRecord temp_record;
	bool hit_any{ false };
	auto closest = tMax;

}

Color RayColor(const Ray& r, ShapeContainer& shapes,const Interval& ray_t, int max_depth)
{
	if (max_depth <= 0)
		return Color(0.0f);
	HitRecord rec;
	auto closest = ray_t.max;
	for (auto& sha : shapes)
	{
		if (sha->RayHit(r,rec, Interval(0.001, closest)))
		{
			Point direction = rec.normal + RandomUnitVector();
			return 0.5f * RayColor(Ray(rec.p, direction), shapes, ray_t, max_depth -1);
			closest = rec.t;
		}
	}
	glm::vec3 direct = glm::normalize(r.GetDirection());
	float a = 0.5f * (direct.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + (a * glm::vec3(0.5, 0.7, 1.0));
}