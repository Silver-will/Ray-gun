#ifndef POLYGUN_H
#define POLYGUN_H
#pragma once
#include "GLTFLoading.h"
#include "Shape.h"
#include "AABB.h"
struct Triangle : public Shape
{
	Triangle(Point V0, Point V1, Point V2, std::shared_ptr<Material> Mat) : v0{ V0 }, v1{ V1 }, v2{ V2 }, mat{ Mat } {
		/*glm::vec3 min(fmin(v0.x, fmin(v1.x, v2.x)),
			fmin(v0.y, fmin(v1.y, v2.y)),
			fmin(v0.z, fmin(v1.z, v2.z)));

		glm::vec3 max(fmax(v0.x, fmax(v1.x, v2.x)),
			fmax(v0.y, fmax(v1.y, v2.y)),
			fmax(v0.z, fmax(v1.z, v2.z)));
		*/
		std::vector<Point> vertices;
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);

		glm::vec3 min = glm::vec3(10000000.0f, 1000000.0f, 1000000.0f);
		glm::vec3 max = glm::vec3(-10000000.0f, -1000000.0f, -1000000.0f);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				min[j] = __min(vertices[i][j], min[j]);
				max[j] = __max(vertices[i][j], max[j]);
			}
		}

		//min = glm::vec3(-450);
		//max = glm::vec3(450);
		BBox = AABB(min, max).Pad();
		//BBox.x.Expand(20);
		//BBox.y.Expand(100);
		//BBox.z.Expand(100);
	}
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t) override;
	Color GetColor() override;
	Point GetPos() override;
	AABB GetBoundingBox()const override;
private:
	AABB BBox;
	Point v0, v1, v2;
	float u, v, t;
	std::shared_ptr<Material> mat;
	Color color = Color(.73, .73, .73);
};

struct Polygun
{
	Polygun(const std::vector<MeshData>& _geometry);
	Color GetColor();
	Point GetPos();
	AABB GetBoundingBox()const;

private:
	std::vector<MeshData> geometry;
	AABB BBox;
	std::shared_ptr<Material> mat;
};
#endif

