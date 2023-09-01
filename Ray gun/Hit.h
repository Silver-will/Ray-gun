#ifndef HIT_RECORD_H
#define HIT_RECORD_H
#include "Typedefs.h"
#include "Ray.h"
struct HitRecord
{
	Point p;
	glm::vec3 normal;
	double t;
	bool frontFace;
	void setFaceNormal(const Ray& r, const glm::vec3& outward);
};
#endif
