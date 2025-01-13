#include "ONB.h"

glm::vec3 ONB::operator[] (int i) const
{
	return axis[i];
}
void ONB::build_from_w(const glm::vec3& w)
{
	glm::vec3 unit_w = glm::normalize(w);
	glm::vec3 a = (fabs(unit_w.x) > 0.9) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
	glm::vec3 v = glm::normalize(cross(unit_w, a));
	glm::vec3 u = cross(unit_w, v);
	axis[0] = u;
	axis[1] = v;
	axis[2] = unit_w;
}

glm::vec3 ONB::u()const
{
	return axis[0];
}

glm::vec3 ONB::v()const
{
	return axis[1];
}

glm::vec3 ONB::w()const
{
	return axis[2];
}

glm::vec3 ONB::local(float a, float b, float c) const
{
	return a * u() + b * v() + c * w();
}

glm::vec3 ONB::local(const glm::vec3& a) const
{
	return a.x * u() + a.y * v() + a.z * w();
}

glm::vec3 ONB::transform(const glm::vec3& v) const {
	// Transform from basis coordinates to local space.
	return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
}