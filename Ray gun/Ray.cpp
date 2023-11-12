#include "Ray.h"
Ray::Ray() : origin{glm::vec3(0.0f)}, direction{glm::vec3(0.0f)},
time{0.0}
{

}

Ray::Ray(glm::vec3 orig, glm::vec3 dir) : origin{orig}, direction{dir},time{0.0}
{
	
}

Ray::Ray(glm::vec3 orig, glm::vec3 dir,double tm) : origin{ orig }, direction{ dir }, time{tm}
{

}

glm::vec3 Ray::At(float t) const
{
	return origin + (t * direction);
}

glm::vec3 Ray::GetDirection() const
{
	return direction;
}

glm::vec3 Ray::GetOrigin() const
{
	return origin;
}

void Ray::SetDirection(const glm::vec3& dir)
{
	direction = dir;
}

void Ray::SetOrigin(const glm::vec3& orig)
{
	origin = orig;
}

float Ray::LengthSquared()
{
	return ((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
}

double Ray::GetTime() const
{
	return time;
}