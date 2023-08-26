#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<fstream>
#include<vector>
#include<memory>
#include "Camera.h"
#include "Shape.h"

struct Image
{
	Image(uint16_t width, double aspectRatio);
	~Image();
private:
	void PrintToFile();
	void SetUpOutputFile();
	void SetUpScene();
	void AddSphere(float rad, Point pos, Color col);
	uint16_t WIDTH;
	uint16_t HEIGHT;
	std::ofstream image;
	double aspect_ratio;
	Camera cam;
	ShapeContainer shapes;
};
#endif
