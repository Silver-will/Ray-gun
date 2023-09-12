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

void Image::AddSphere(float rad, Point pos)
{
	shapes.emplace_back(std::make_shared<Sphere>(pos, rad));
}

Image::~Image()
{
	image.close();
}

void Image::PrintToFile()
{
	sample_count = 10;
	image << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

	for (size_t j = 0; j < HEIGHT; j++)
	{
		std::cout << "Lines left = " << HEIGHT - j << "\n";
		for (size_t i = 0; i < WIDTH; i++)
		{
			Color pixel_color = Color(0.0f);
			for (size_t sample = 0; sample < sample_count; sample++)
			{
				auto ray = cam.GetRay(i, j);
			    pixel_color += RayColor(ray, shapes, Interval(0, Common::infinity),40);
			}
			WriteColor(image, pixel_color, sample_count);
		}
	}
}

void Image::SetUpScene()
{
	AddSphere(0.6f, Point(0.0f,0.0f,-1.0f));
	AddSphere(100.0f, Point(0, -100.5, -1));
}