#ifndef RAY_H
#define RAY_H
#include<glm/glm.hpp>
#include"Typedefs.h"
struct Ray
{
	
	Ray();
	Ray(glm::vec3 orig, glm::vec3 dir);
	glm::vec3 At(float t);
	glm::vec3 GetDirection() const;
	glm::vec3 GetOrigin() const;
	void SetDirection(const glm::vec3& dir);
	void SetOrigin(const glm::vec3& orig);
	float LengthSquared();
private:
	glm::vec3 origin;
	glm::vec3 direction;
};
#endif
