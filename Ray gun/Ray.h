#ifndef RAY_H
#define RAY_H
#include<glm/glm.hpp>
#include"Typedefs.h"
struct Ray
{
	
	Ray();
	Ray(glm::vec3 orig, glm::vec3 dir);
	Ray(glm::vec3 orig, glm::vec3 dir, double tm);
	glm::vec3 At(float t)const;
	glm::vec3 GetDirection() const;
	glm::vec3 GetOrigin() const;
	double GetTime() const;
	void SetDirection(const glm::vec3& dir);
	void SetOrigin(const glm::vec3& orig);
	float LengthSquared();
private:
	glm::vec3 origin;
	glm::vec3 direction;
	double time;
};
#endif
