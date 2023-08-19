#include "Color.h"
#include<fstream>
#include<iostream>
void WriteColor(std::ofstream& out, const color& col)
{
	out << static_cast<int>(255.999 * col.x) << ' '
		<< static_cast<int>(255.999 * col.y) << ' '
		<< static_cast<int>(255.999 * col.z) << "\n";
}

color RayColor(Ray& r)
{
	glm::vec3 direct = glm::normalize(r.GetDirection());
	float a = 0.5f * (direct.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + (a * glm::vec3(0.5, 0.7, 1.0));
}