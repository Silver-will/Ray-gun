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

Color RayColor(Ray& r, ShapeContainer& shapes,const Interval& ray_t)
{
	HitRecord rec;
	auto closest = ray_t.max;
	for (auto& sha : shapes)
	{
		if (sha->RayHit(r,rec, Interval(ray_t.min, closest)))
		{
			return 0.5f * (rec.normal + Color(1, 1, 1));
			closest = rec.t;
		}
	}
	glm::vec3 direct = glm::normalize(r.GetDirection());
	float a = 0.5f * (direct.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + (a * glm::vec3(0.5, 0.7, 1.0));
}