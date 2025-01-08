#include "BVH.h"
#include <algorithm>
bool BVH_Node::RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)
{
	if(!box.Hit(r,ray_t))
		return false;
	bool hit_left = left->RayHit(r, hit, ray_t);
	bool hit_right = right->RayHit(r, hit, Interval(ray_t.min, hit_left ? hit.t : ray_t.max));
	return
		hit_left || hit_right;
}

AABB BVH_Node::GetBoundingBox() const
{
	return box;
}

BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Shape>>& srcObjects, size_t start, size_t end)
{
	auto objects = srcObjects;
	int axis = random_int(0, 2);

	auto comparator = (axis == 0) ? box_compare_X
		: (axis == 1) ? box_compare_Y
		: box_compare_Z;
	size_t object_span = end - start;
	if (object_span == 1) 
	{
		left = right = objects[start];
	}
	else if(object_span == 2) 
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		auto mid = start + object_span /2;
		left = std::make_shared <BVH_Node>(objects, start, mid);
		right = make_shared<BVH_Node>(objects, mid, end);
	}
	box = AABB(left->GetBoundingBox(), right->GetBoundingBox());
}

bool BVH_Node::box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axisIndex)
{
	return a->GetBoundingBox().axis(axisIndex).min < b->GetBoundingBox().axis(axisIndex).min;
}

bool BVH_Node::box_compare_X(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b)
{
	return box_compare(a, b, 0);
}

bool BVH_Node::box_compare_Y(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b)
{
	return box_compare(a, b, 1);
}

bool BVH_Node::box_compare_Z(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b)
{
	return box_compare(a, b, 2);
}

BVH_Node::BVH_Node(ShapeList& shapes) : BVH_Node(shapes.objects,0,shapes.objects.size())
{

}