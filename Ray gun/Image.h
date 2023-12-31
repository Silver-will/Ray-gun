#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<fstream>
#include<vector>
#include<memory>
#include "Camera.h"
#include "Shape.h"
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
	void SetCameraFocusValues(float defocus_angle, float focus_distance);
	void AddSphere(float rad, Point pos, Color col);
	void AddMetal(float rad,float fuzz, Point pos, Color col);
	void AddLambder(float rad, Point pos, std::shared_ptr<Texture> col);
	void AddLambderQuad(Point Q, Point U,Point V, std::shared_ptr<Material> m);
	void AddLambder(float rad, Point pos, Point pos2, Color col);
	void AddDielectric(float rad, Point pos, double refractive_index);
	AABB GetShapeBox();
	uint16_t WIDTH;
	uint16_t HEIGHT;
	std::ofstream image;
	double aspect_ratio;
	Camera cam;
	ShapeContainer shapes;
	AABB shape_box;
	int scene = 6;
	int sample_count = 200;
};
#endif

