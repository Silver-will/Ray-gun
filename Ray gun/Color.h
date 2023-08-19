#ifndef COLOR_H
#define COLOR_H
#include<glm/glm.hpp>
#include<iostream>
#include"Ray.h";
using color = glm::vec3;

void WriteColor(std::ofstream& out, const color& col);
color RayColor(Ray& r);
#endif

