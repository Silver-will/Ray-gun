#ifndef AABB_H
#define AABB_H
#include "Interval.h"
#include "Typedefs.h"
#include "Ray.h"
struct AABB
{
	Interval x, y, z;
	AABB() {}
	AABB(Interval X, Interval Y, Interval Z) :x{ X }, y{ Y }, z{ Z } {}
	AABB(const Point& a,const Point& b);
	AABB(const AABB& a, const AABB& b);
	AABB Pad();

	const Interval& axis(int n) const;
	bool Hit(const Ray& r, Interval ray_t)const;
};
#endif


