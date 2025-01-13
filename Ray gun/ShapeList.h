#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include "AABB.h"
#include "Shape.h"

#include<memory>
struct ShapeList :  public Shape
{
	std::vector<std::shared_ptr<Shape>> objects;

	ShapeList() {}
	ShapeList(std::shared_ptr<Shape> object);

	void Clear();
	void Add(std::shared_ptr<Shape> object);
	
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const override;
	double PDFValue(const Point& origin, const Point& direction) const override;
	glm::vec3 Random(const Point& origin)const override;
	AABB GetBoundingBox() const;

private:
	AABB Bbox;
};
#endif

