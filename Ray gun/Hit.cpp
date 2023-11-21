#include "Hit.h"

void HitRecord::SetFaceNormal(const Ray& r, const glm::vec3& outward)
{
	frontFace = glm::dot(r.GetDirection(), outward) < 0;
	normal = frontFace ? outward : -outward;
}