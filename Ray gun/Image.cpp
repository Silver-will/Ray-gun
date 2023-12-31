#include "Image.h"
#include "Color.h"
#include "Material.h"
#include "BVH.h"
#include "Quad.h"

#include<chrono>
using namespace std::literals;
Image::Image(uint16_t width, double aspectRatio)
{
	WIDTH = width;
	auto h = static_cast<int>(WIDTH / aspectRatio);
	HEIGHT = (h > 0) ? h : 1;
	cam = Camera(HEIGHT, WIDTH);
	switch (scene)
	{
	case 1:
		SetCameraFocusValues(0.6, 10.0);
		cam.setCameraAngle(Point(13, 2, 3), Point(0, 0, 0), Point(0, 1, 0), 20.0);
		SetUpRandomBallScene();
		break;
	case 2:
		SetCameraFocusValues(0, 10.0);
		cam.setCameraAngle(Point(13, 2, 3), Point(0, 0, 0), Point(0, 1, 0), 20.0);
		SetUpSphereScene();
		break;
	case 3:
		SetCameraFocusValues(0.6, 10.0);
		cam.setCameraAngle(Point(13, 2, 3), Point(0, 0, 0), Point(0, 1, 0), 20.0);
		SetUpEarthScene();
		break;
	case 4:
		SetCameraFocusValues(0, 10.0);
		cam.setCameraAngle(Point(13, 2, 3), Point(0, 0, 0), Point(0, 1, 0), 20.0);
		SetUpNoiseScene();
		break;
	case 5:
		SetCameraFocusValues(0, 10.0);
		cam.setCameraAngle(Point(0, 0, 9), Point(0, 0, 0), Point(0, 1, 0), 80.0);
		SetUpQuads();
		break;
	case 6:
		SetCameraFocusValues(0, 10.0);
		cam.setCameraAngle(Point(278, 278, -800), Point(278, 278, 0), Point(0, 1, 0), 40.0);
		SetUpCornellBox();
		break;
	default:
		break;
	}
	SetUpOutputFile();
	PrintToFile();
}

void Image::SetUpOutputFile()
{
	std::string file = "./Render/Scene.ppm";
	image.open(file, std::ofstream::out | std::ofstream::binary);
}

void Image::AddSphere(float rad, Point pos, Color col)
{
	shapes.emplace_back(std::make_shared<Sphere>(pos, rad, col));
}

void Image::AddMetal(float rad, float fuzz, Point pos, Color col)
{
	auto metal = std::make_shared<Metal>(col,fuzz);
	shapes.emplace_back(std::make_shared<Sphere>(pos, rad, metal));
	shape_box = AABB(shape_box,shapes.back()->GetBoundingBox());
}

void Image::AddLambder(float rad, Point pos, std::shared_ptr<Texture> col)
{
	auto lambder = std::make_shared<Lambertian>(col);
	shapes.emplace_back(std::make_shared<Sphere>(pos, rad, lambder));
	shape_box = AABB(shape_box, shapes.back()->GetBoundingBox());
}

void Image::AddLambder(float rad, Point pos, Point pos2, Color col)
{
	auto lambder = std::make_shared<Lambertian>(col);
	shapes.emplace_back(std::make_shared<Sphere>(pos,pos2, rad, lambder));
	shape_box = AABB(shape_box, shapes.back()->GetBoundingBox());
}


void Image::AddLambderQuad(Point Q, Point U, Point V, std::shared_ptr<Material> m)
{
	shapes.emplace_back(std::make_shared<Quad>(Q, U, V, m));
	shape_box = AABB(shape_box, shapes.back()->GetBoundingBox());
}

void Image::AddDielectric(float rad, Point pos, double refractive_index)
{
	auto dielectric = std::make_shared<Dielectric>(refractive_index);
	shapes.emplace_back(std::make_shared<Sphere>(pos, rad, dielectric));
	shape_box = AABB(shape_box, shapes.back()->GetBoundingBox());
}

Image::~Image()
{
	image.close();
}

void Image::PrintToFile()
{

	image << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";
	auto draw_start = std::chrono::steady_clock::now();
	for (size_t j = 0; j < HEIGHT; j++)
	{
		std::cout << "Lines left = " << HEIGHT - j << "\n";
		for (size_t i = 0; i < WIDTH; i++)
		{
			Color pixel_color = Color(0.0f);
			for (size_t sample = 0; sample < sample_count; sample++)
			{
				auto ray = cam.GetRay(i, j);
			    pixel_color += RayColor(ray, shapes, 50);
			}
			WriteColor(image, pixel_color, sample_count);
		}
	}
	auto draw_end = std::chrono::steady_clock::now();
	std::cout << "Total time taken: " << (draw_end - draw_start) / 1s <<" seconds" << std::endl;;
}

void Image::SetUpSphereScene()
{
	auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9));
	AddLambder(10.0f, Point(0, -10, -1.0), checker);
	AddLambder(10.0f, Point(0, 10, -1.0), checker);
	auto volumes = std::make_shared<BVH_Node>(shapes);
	shapes = ShapeContainer(1, volumes);

}

void Image::SetUpRandomBallScene()
{
	auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2,.3,.1),Color(.9));
	AddLambder(1000.0f, Point(0, -1000, -1.0), checker);
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double choose_mat = random_double();
			Point center = Point(a + 0.9f * random_double(), 0.2f, b + 0.9f * random_double());

			if (glm::length(center - Point(4, 0.2, 0)) > 0.9f)
			{
				if (choose_mat < 0.8f)
				{
					auto albedo = RandomVector() * RandomVector();
					auto center2 = center + Point(0, random_double(0, .5), 0);
					AddLambder(0.2f, center,center2, albedo);
				}
				else if (choose_mat < 0.95)
				{
					auto albedo = RandomVector(0.5,1);
					auto fuzz = random_double(0,0.5);
					AddMetal(0.2f, fuzz, center, albedo);
				}
				else
				{
					AddDielectric(0.2f, center, 1.5f);
				}
			}
		}
	}
	AddLambder(1.0f, Point(-4, 1, 0), checker);
	AddDielectric(1.0f, Point(0, 1, 0), 1.5f);
	AddMetal(1.0f, 0.0f, Point(4, 1, 0), Color(0.7, 0.6, 0.5));
	

	auto volumes = std::make_shared<BVH_Node>(shapes);
	shapes = ShapeContainer(1,volumes);
}

AABB Image::GetShapeBox()
{
	return shape_box;
}

void Image::SetCameraFocusValues(float defocus_angle, float focus_distance)
{
	cam.defocus_angle = defocus_angle;
	cam.focus_dist = focus_distance;
}

void Image::SetUpEarthScene()
{
	auto earthTex = std::make_shared<ImageTexture>("earthmap.jpg");
	AddLambder(2.0 , Point(0), earthTex);
	auto volumes = std::make_shared<BVH_Node>(shapes);
	shapes = ShapeContainer(1, volumes);
}

void Image::SetUpNoiseScene()
{
	auto Noise = std::make_shared<NoiseTexture>(4);
	AddLambder(1000.0f, Point(0, -1000, -1.0), Noise);
	AddLambder(2.0f, Point(0, 2, 0), Noise);
	auto volumes = std::make_shared<BVH_Node>(shapes);
	shapes = ShapeContainer(1, volumes);
}

void Image::SetUpQuads()
{ 
	auto leftRed = std::make_shared<Lambertian>(Color(1, 0, 0));
	auto backGreen = std::make_shared<Lambertian>(Color(0, 1, 0));
	auto rightBlue = std::make_shared<Lambertian>(Color(0, 0, 1));
	auto upperOrange = std::make_shared<Lambertian>(Color(1, 0.5, 0));
	auto lowerTeal = std::make_shared<Lambertian>(Color (0.2, 0.8, 0.8));

	AddLambderQuad(Point(-3, -2, 5), Point(0, 0, -4), Point(0, 4, 0), leftRed);
	AddLambderQuad(Point(-2, -2, 0), Point(4, 0, 0), Point(0, 4, 0), backGreen);
	AddLambderQuad(Point(3, -2, 1), Point(0, 0, 4), Point(0, 4, 0), rightBlue);
	AddLambderQuad(Point(-2, 3, 1), Point(4, 0, 0), Point(0, 0, 4), upperOrange);
	AddLambderQuad(Point(-2, -3, 5), Point(4, 0, 0), Point(0, 0, -4), lowerTeal);

	auto volumes = std::make_shared<BVH_Node>(shapes);
	shapes = ShapeContainer(1, volumes);
}

void Image::SetUpCornellBox()
{
	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto light = std::make_shared<DiffuseLight>(Color(15,15,15));

	AddLambderQuad(Point(555, 0, 0), Point(0,555,0), Point(0,0,555), green);
	AddLambderQuad(Point(0, 0, 0), Point(0, 555, 0), Point(0, 0, 555), red);
	AddLambderQuad(Point(343, 554,332), Point(-130, 0, 0), Point(0, 0, -105), light);
	AddLambderQuad(Point(0, 0, 0), Point(555, 0, 0), Point(0, 0, 555), white);
	AddLambderQuad(Point(555, 555, 555), Point(-555, 0, 0), Point(0, 0, -555), white);
	AddLambderQuad(Point(0, 0, 555), Point(555, 0, 0), Point(0, 555, 0), white);
}