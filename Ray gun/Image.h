#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<fstream>
#include "Camera.h"
struct Image
{
	Image(uint16_t width, double aspectRatio);
	~Image();
private:
	void PrintToFile();
	void SetUpOutputFile();
	uint16_t WIDTH;
	uint16_t HEIGHT;
	std::ofstream image;
	double aspect_ratio;
	Camera cam;
};
#endif

