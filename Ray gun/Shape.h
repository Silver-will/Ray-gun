#ifndef SPHERE_H
#define SPHERE_H
#include"Ray.h"

struct Shape
{
	virtual double RayHit(Ray& r) = 0;
	virtual Color GetColor() = 0;
	virtual Point GetPos() = 0;
};

struct Sphere : public Shape
{
	Sphere();
	Sphere(Point orig, float rad, Color col);
	double RayHit(Ray& r) override;
	Color GetColor() override;
	Point GetPos() override;
private:
	Point Center;
	Color color;
	float radius;
};

using ShapeContainer = std::vector<std::unique_ptr<Shape>>;
#endif

