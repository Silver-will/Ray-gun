#ifndef BVH_H
#define BVH_H
#include"Typedefs.h"

#include "ShapeList.h"

struct BVH_Node : public Shape
{
	BVH_Node(ShapeList& shapes);
	BVH_Node(const std::vector<std::shared_ptr<Shape>>& srcObjects, size_t start, size_t end);
	static bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axisIndex);
	static bool box_compare_X(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
	static bool box_compare_Y(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
	static bool box_compare_Z(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const override;
	AABB GetBoundingBox() const;
	
private:
	std::shared_ptr<Shape> left;
	std::shared_ptr<Shape> right;
	AABB box;
};

#endif // !BVH_H
