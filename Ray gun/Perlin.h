#ifndef PERLIN_H
#define PERLIN_H
#include "Typedefs.h"
struct Perlin
{
	Perlin();
	~Perlin();
	double Noise(const Point& p)const;
	double Turb(const Point& p, int depth = 7)const;


private:
	static const int pointCount = 256;
	glm::vec3* ranVec;
	int* permX;
	int* permY;
	int* permZ;
	static int* PerlinGeneratePerm();
	static void Permute(int* p, int n);
	static double PerlinInterp(Point c[2][2][2], double u, double v, double w);
};
#endif

