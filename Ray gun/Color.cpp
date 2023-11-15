#include<fstream>
#include<iostream>

#include "Color.h"
#include"Common.h"
#include"Material.h"

void AnyHit(ShapeContainer& shapes, Ray& r, HitRecord& hit, double& tMin, double& tMax)
{
	HitRecord temp_record;
	bool hit_any{ false };
	auto closest = tMax;

}


bool WorldHit(const Ray& r, ShapeContainer& shapes, const Interval& ray_t, HitRecord& rec)
{
	bool hit = false;
	auto closest = ray_t.max;
	HitRecord temp;
	for (auto& sha : shapes)
	{
		if (sha->RayHit(r, temp, Interval(ray_t.min, closest)))
		{
			closest = temp.t;
			rec = temp;
			hit = true;
		}
	}
	return hit;
}
