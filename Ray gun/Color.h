#ifndef COLOR_H
#define COLOR_H
#include<glm/glm.hpp>
#include<iostream>
#include<span>
#include "Shape.h"

void WriteColor(std::ofstream& out, const Color& col);
Color RayColor(Ray& r, ShapeContainer& shapes);
#endif

