#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<fstream>
#include<vector>
#include<memory>
#include "Camera.h"
#include "ShapeList.h"
#include "Texture.h"

struct Image
{
	Image(uint16_t width, double aspectRatio);
	~Image();
private:
	void PrintToFile();
	void SetUpOutputFile();
	void SetUpRandomBallScene();
	void SetUpSphereScene();
	void SetUpEarthScene();
	void SetUpNoiseScene();
	void SetUpQuads();
	void SetUpCornellBox();
	void SetUpCornellSmoke();
	void SetUpGltfScene();
	void FinalScene();
	void SetCameraFocusValues(float defocus_angle, float focus_distance);
	void StoreColor(Color col, int x, int y);
	AABB GetShapeBox();
	uint16_t WIDTH;
	uint16_t HEIGHT;
	std::ofstream image;
	double aspect_ratio;
	Camera cam;
	ShapeList shapes;
	AABB shape_box;
	int scene = 6;
	int sample_count = 300;
	int ray_depth = 8;
	std::vector<size_t> image_horizontal_iterator, image_vertical_iterator;
	std::vector<Color> colors;
};
#endif

