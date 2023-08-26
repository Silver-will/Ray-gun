#include<fstream>
#include<iostream>

#include "Color.h"

void WriteColor(std::ofstream& out, const Color& col)
{
	out << static_cast<int>(255.999 * col.x) << ' '
		<< static_cast<int>(255.999 * col.y) << ' '
		<< static_cast<int>(255.999 * col.z) << "\n";
}

Color RayColor(Ray& r, ShapeContainer& shapes)
{
	for (auto& sha : shapes)
	{
		auto t = sha->RayHit(r);
		if (t > 0.0)
		{
			glm::vec3 N = glm::normalize(r.At(t) - glm::vec3(0, 0, -1));
			return 0.5f * Color(N.x + 1, N.y + 1, N.z + 1);
		}
	}
	glm::vec3 direct = glm::normalize(r.GetDirection());
	float a = 0.5f * (direct.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + (a * glm::vec3(0.5, 0.7, 1.0));
}