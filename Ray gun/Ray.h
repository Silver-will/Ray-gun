#ifndef RAY_H
#define RAY_H
#include<glm/glm.hpp>
struct Ray
{
	
	Ray();
	Ray(glm::vec3 orig, glm::vec3 dir);
	glm::vec3 At(float t);
	glm::vec3 GetDirection();
	glm::vec3 GetOrigin();
	void SetDirection(const glm::vec3& dir);
	void SetOrigin(const glm::vec3& orig);
private:
	glm::vec3 origin;
	glm::vec3 direction;
};
#endif
