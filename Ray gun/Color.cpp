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

/*Color RayColor(const Ray& r, ShapeContainer& shapes, int max_depth)
{
	HitRecord rec;
	if (max_depth <= 0)
		return Color(0.0f);
	if (WorldHit(r, shapes, Interval(0.001,Common::infinity), rec))
	{
		Ray scattered;
		Color attenuation;
		if (rec.mat->Scatter(r, rec, attenuation, scattered))
		{
			return attenuation * RayColor(scattered, shapes, max_depth - 1);
		}
		return Color(0.0f);
	}
	glm::vec3 direct = glm::normalize(r.GetDirection());
	float a = 0.5f * (direct.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + (a * glm::vec3(0.5, 0.7, 1.0));
}
*/
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
