#ifndef SPHERE_H
#define SPHERE_H
#include"Ray.h"
#include "Hit.h"
#include"Interval.h"

struct Shape
{
	virtual bool RayHit(Ray& r, HitRecord& hit,const Interval& ray_t) = 0;
	virtual Color GetColor() = 0;
	virtual Point GetPos() = 0;
};

struct Sphere : public Shape
{
	Sphere();
	Sphere(Point orig, float rad, Color col);
	Sphere(Point orig, float rad);
	bool RayHit(Ray& r, HitRecord& hit,const Interval& ray_t) override;
	Color GetColor() override;
	Point GetPos() override;
private:
	Point Center;
	Color color;
	float radius;
};

using ShapeContainer = std::vector<std::shared_ptr<Shape>>;
#endif

