#include "AABB.h"

AABB::AABB(const Point& a,const Point& b)
{
	x = Interval(fmin(a[0], b[0]),fmax(a[0], b[0]));
	y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
	z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
}

AABB::AABB(const AABB& a, const AABB& b)
{
	x = Interval(a.x, b.x);
	y = Interval(a.y, b.y);
	z = Interval(a.z, b.z);
}

const Interval& AABB::axis(int n) const
{
	if (n == 1) return y;
	if (n == 2) return z;
	return x;
}

bool AABB::Hit(const Ray& r, Interval ray_t)const
{
	for(int a = 0; a < 3; a++) 
	{
		auto invD = 1/ r.GetDirection()[a];
		auto orig = r.GetOrigin()[a];
		auto t0 = (axis(a).min - orig) * invD;
		auto t1 = (axis(a).max - orig) * invD;
		if (invD < 0)
		std::swap(t0, t1);

		if (t0 > ray_t.min)
		ray_t.min = t0;

		if(t1 < ray_t.max)
		ray_t.max = t1;

		if(ray_t.max <= ray_t.min)
			return false;
	}
	return true;
}

AABB AABB::Pad()
{
	double delta = 0.0001;
	Interval new_x = (x.Size() >= delta) ? x : x.Expand(delta);
	Interval new_y = (y.Size() >= delta) ? y : y.Expand(delta);
	Interval new_z = (z.Size() >= delta) ? z : z.Expand(delta);

	return AABB(new_x, new_y, new_z);
}

AABB operator+(const AABB& bbox, const glm::vec3& offset)
{
	return AABB(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z);
}

AABB operator+(const glm::vec3& offset, const AABB& bbox)
{
	return bbox + offset;
}