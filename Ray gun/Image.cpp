#include "Image.h"
#include "Color.h"

Image::Image(uint16_t width, double aspectRatio)
{
	WIDTH = width;
	auto h = static_cast<int>(WIDTH / aspectRatio);
	HEIGHT = (h > 0) ? h : 1;
	cam = Camera(HEIGHT, WIDTH);
	SetUpOutputFile();
	SetUpScene();
	PrintToFile();
}

void Image::SetUpOutputFile()
{
	std::string file = "./Scene.ppm";
	image.open(file, std::ofstream::out | std::ofstream::binary);
}

void Image::AddSphere(float rad, Point pos, Color col)
{
	shapes.emplace_back(std::make_unique<Sphere>(pos, rad, col));
}

Image::~Image()
{
	image.close();
}

void Image::PrintToFile()
{
	image << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

	for (size_t j = 0; j < HEIGHT; j++)
	{
		std::cout << "Lines left = " << HEIGHT - j << "\n";
		for (size_t i = 0; i < WIDTH; i++)
		{
			auto ray_dir = cam.GetPixelCenter(i,j) - cam.GetCameraOrigin();
			Ray r(cam.GetCameraOrigin(), ray_dir);
			Color pixel_color = RayColor(r, shapes);
			WriteColor(image, pixel_color);
		}
	}
}

void Image::SetUpScene()
{
	AddSphere(0.6f, Point(0.0f,0.0f,-1.0f), Color(0.0f, 0.0f,0.5f));
}