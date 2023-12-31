#ifndef HIT_RECORD_H
#define HIT_RECORD_H
#include "Typedefs.h"
#include "Ray.h"
#include<memory>
struct Material;
struct HitRecord
{
	Point p;
	glm::vec3 normal;
	std::shared_ptr<Material>mat;
	double t;
	double u;
	double v;
	bool frontFace;
	void SetFaceNormal(const Ray& r, const glm::vec3& outward);
};
#endif
