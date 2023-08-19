#include "Ray.h"
Ray::Ray()
{

}

Ray::Ray(glm::vec3 orig, glm::vec3 dir) : origin{orig}, direction{dir}
{
	
}

glm::vec3 Ray::At(float t)
{
	return origin + (t * direction);
}

glm::vec3 Ray::GetDirection()
{
	return direction;
}

glm::vec3 Ray::GetOrigin()
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