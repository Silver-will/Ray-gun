#ifndef HIT_RECORD_H
#define HIT_RECORD_H
#include "Typedefs.h"
#include "Ray.h"
struct Material;
struct HitRecord
{
	Point p;
	glm::vec3 normal;
	std::shared_ptr<Material>mat;
	double t;
	bool frontFace;
	void setFaceNormal(const Ray& r, const glm::vec3& outward);
};
#endif
