#ifndef POLYGUN_H
#define POLYGUN_H
#pragma once
#include "ShapeList.h"
#include "Material.h"
#include "GLTFLoading.h"
#include "ShapeList.h"
#include "AABB.h"
struct Triangle : public Shape
{
	Triangle(Point V0, Point V1, Point V2, std::shared_ptr<Material> Mat) : v0{ V0 }, v1{ V1 }, v2{ V2 }, mat{ Mat } {
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
		//BBox = AABB(glm::vec3(138), glm::vec3(450));
		BBox = AABB(min, max).Pad();
		BBox.x = BBox.x.Expand(1);
		BBox.y = BBox.y.Expand(1);
		BBox.z = BBox.z.Expand(1);
	}

	Triangle(const Triangle& tri);
	void Translate(const glm::vec3& direction);
	void Scale(const glm::vec3& scale);
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const override;
	Color GetColor() override;
	Point GetPos() override;
	AABB GetBoundingBox()const override;
	double PDFValue(const Point& origin, const Point& direction) const override;
	glm::vec3 Random(const Point& origin)const override;
	float Area() const;
	AABB BBox;
	Point v0, v1, v2;
	//float u, v, t;
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
	Point world_position;
};
#endif

