#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H
#include "Shape.h"
#include "Texture.h"

struct ConstantMedium : public Shape
{
	ConstantMedium(std::shared_ptr<Shape> b, double d, std::shared_ptr<Texture> a);
	ConstantMedium(std::shared_ptr<Shape> b, double d, Color c);
	bool RayHit(const Ray& r, HitRecord& hit, const Interval& ray_t) override;
	AABB GetBoundingBox() const;
private:
	std::shared_ptr<Shape> boundary;
	double negInvDensity;
	std::shared_ptr<Material> phaseFunction;
};
#endif

