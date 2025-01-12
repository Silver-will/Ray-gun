#ifndef SHAPE_H
#define SHAPE_H
#include"Ray.h"
#include "Hit.h"
#include"Interval.h"
#include "AABB.h"
#include<vector>

struct Shape
{
	virtual bool RayHit(const Ray& r, HitRecord& hit,const Interval& ray_t)const = 0;
	virtual Color GetColor() { return Color(0); }
	virtual Point GetPos() { return Point(0); }
	virtual double PDFValue(const Point& origin, const Point& direction) const {
		return 0.0;
	}
	virtual glm::vec3 Random(const Point& origin)const {
		return glm::vec3(1, 0, 0);
	}
	virtual AABB GetBoundingBox()const = 0;
};

struct Sphere : public Shape
{

	Sphere(Point orig, float rad, Color col);
	Sphere(Point orig, float rad, std::shared_ptr<Material> Mat);
	Sphere(Point orig,Point orig1, float rad, std::shared_ptr<Material> Mat);
	bool RayHit(const Ray& r, HitRecord& hit,const Interval& ray_t)const override;
	Color GetColor() override;
	Point GetPos() override;
	Point LerpCenter(double time)const;
	AABB GetBoundingBox()const override;
private:
	static void GetSphereUV(const Point& p, double& u, double& v);
	std::shared_ptr<Material> mat;
	Point center;
	Point centerVec;
	Color color;
	float radius;
	bool isMoving;
	AABB BBox;
};

struct Translate : public Shape
{
	Translate(std::shared_ptr<Shape> s, const glm::vec3& displacement);
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const override;
	AABB GetBoundingBox()const override;

private:
	AABB box;
	glm::vec3 offset;
	std::shared_ptr<Shape> object;
};

struct RotateY : public Shape
{
	RotateY(std::shared_ptr<Shape> p, double angle);
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t)const override;
	AABB GetBoundingBox()const override;

private:
	AABB box;
	float sin_theta;
	float cos_theta;
	std::shared_ptr<Shape>object;
};
#endif

