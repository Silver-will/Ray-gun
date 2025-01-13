#ifndef ONB_H
#define ONB_H
#include<glm/glm.hpp>
struct ONB
{
	ONB() {}
	glm::vec3 operator[] (int i) const;
	//glm::vec3& operator[] (int i) const;

	
	glm::vec3 u()const;
	glm::vec3 v()const;
	glm::vec3 w()const;

	glm::vec3 local(float a, float b, float c) const;

	glm::vec3 local(const glm::vec3& a) const;
	glm::vec3 transform(const glm::vec3& v) const;
	void build_from_w(const glm::vec3& w);
private:
	glm::vec3 axis[3];
};
#endif
