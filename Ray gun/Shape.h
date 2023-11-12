#ifndef SPHERE_H
#define SPHERE_H
#include"Ray.h"
#include "Hit.h"
#include"Interval.h"
#include "AABB.h"
#include<vector>

struct Shape
{
	virtual bool RayHit(const Ray& r, HitRecord& hit,const Interval& ray_t) = 0;
	virtual Color GetColor() = 0;
	virtual Point GetPos() = 0;
	virtual AABB GetBoundingBox()const = 0;
};

struct Sphere : public Shape
{

	Sphere(Point orig, float rad, Color col);
	Sphere(Point orig, float rad, std::shared_ptr<Material> Mat);
	Sphere(Point orig,Point orig1, float rad, std::shared_ptr<Material> Mat);
	bool RayHit(const Ray& r, HitRecord& hit,const Interval& ray_t) override;
	Color GetColor() override;
	Point GetPos() override;
	Point LerpCenter(double time);
	AABB GetBoundingBox()const override;
private:
	std::shared_ptr<Material> mat;
	Point center;
	Point centerVec;
	Color color;
	float radius;
	bool isMoving;
	AABB BBox;
};



using ShapeContainer = std::vector<std::shared_ptr<Shape>>;
#endif

