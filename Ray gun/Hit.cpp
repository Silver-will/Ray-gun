#include "Hit.h"

void HitRecord::setFaceNormal(const Ray& r, const glm::vec3& outward)
{
	frontFace = glm::dot(r.GetDirection(), outward);
	normal = frontFace ? outward : -outward;
}