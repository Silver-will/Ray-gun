#ifndef POLYGUN_H
#define POLYGUN_H
#pragma once
#include "GLTFLoading.h"
#include "ShapeList.h"
#include "AABB.h"

struct Triangle : public Shape
{
	Triangle(Point V0, Point V1, Point V2, std::shared_ptr<Material> Mat) : v0{ V0 }, v1{ V1 }, v2{ V2 }, mat{ Mat } {
		glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
		glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		auto minx = fmin(v0.x, fmin(v1.x, v2.x));
		auto miny = fmin(v0.y, fmin(v1.y, v2.y));
		auto minz = fmin(v0.z, fmin(v1.z, v2.z));

		auto maxx = fmax(v0.x, fmax(v1.x, v2.x));
		auto maxy = fmax(v0.y, fmax(v1.y, v2.y));
		auto maxz = fmax(v0.z, fmax(v1.z, v2.z));

		min.x = fmin(minx, min.x);
		min.y = fmin(miny, min.y);
		min.z = fmin(minz, min.z);

		max.x = fmax(maxx, max.x);
		max.y = fmax(maxy, max.y);
		max.z = fmax(maxz, max.z);

		/*
		glm::vec3 min(fmin(v0.x, fmin(v1.x, v2.x)),
			fmin(v0.y, fmin(v1.y, v2.y)),
			fmin(v0.z, fmin(v1.z, v2.z)));

		glm::vec3 max(fmax(v0.x, fmax(v1.x, v2.x)),
			fmax(v0.y, fmax(v1.y, v2.y)),
			fmax(v0.z, fmax(v1.z, v2.z)));
		*/
		BBox = AABB(min, max).Pad();
	}

	Triangle(const Triangle& tri);
	void Translate(const glm::vec3& direction);
	void Scale(const glm::vec3& scale);
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t) override;
	Color GetColor() override;
	Point GetPos() override;
	AABB GetBoundingBox()const override;

	AABB BBox;
	Point v0, v1, v2;
	float u, v, t;
	std::shared_ptr<Material> mat;
	Color color = Color(.73, .73, .73);
};

struct Polygun
{
	Polygun(const std::vector<MeshData>& _geometry);
	Polygun(std::string_view model_path, Point pos);
	void AddToScene(ShapeList& shapes);
	Color GetColor();
	Point GetPos();

private:
	std::vector<MeshData> geometry;
	std::vector<float> vertices;
	std::vector<float> indices;
	AABB BBox;
	std::shared_ptr<Material> mat;
	Point worldPos;
};
#endif

