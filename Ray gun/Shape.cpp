#include "Shape.h"
#include<iostream>
Sphere::Sphere(Point orig, float rad, Color col) : Center{orig}, 
	radius{rad}, color{col}
{

}

Sphere::Sphere() : Center{glm::vec3(0.0f)}, radius{0.3f},
	color{glm::vec3(0.0f)}
{

}
double Sphere::RayHit(Ray& r)
{
	glm::vec3 oc = r.GetOrigin() - Center;
	auto a = glm::dot(r.GetDirection(), r.GetDirection());
	auto half_b = glm::dot(oc, r.GetDirection());
	auto c = glm::dot(oc, oc) - radius * radius;
	auto discriminant = (half_b * half_b) - ( a * c);
	
	if (discriminant < 0.0f)
	{
		return -1.0f;
	}
	else
	{
		return (-half_b - sqrt(discriminant)) / a;
	}
}

Color Sphere::GetColor()
{
	return color;
}

Point Sphere::GetPos()
{
	return Center;
}