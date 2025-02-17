#include "Image.h"
#include "Color.h"
#include "Material.h"
#include "BVH.h"
#include "Quad.h"
#include "ShapeList.h"
#include "Polygun.h"
#include "ConstantMedium.h"
#include "GLTFLoading.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <execution>
#include<chrono>
using namespace std::literals;
Image::Image(uint16_t width, double aspectRatio)
{
	WIDTH = width;
	auto h = static_cast<int>(WIDTH / aspectRatio);
	HEIGHT = (h > 0) ? h : 1;
	colors.resize(WIDTH * HEIGHT);
	image_horizontal_iterator.resize(WIDTH);
	image_vertical_iterator.resize(HEIGHT);

	for (size_t i = 0; i < WIDTH; i++)
		image_horizontal_iterator[i] = i;
	for (size_t i = 0; i < HEIGHT; i++)
		image_vertical_iterator[i] = i;

	cam = Camera(HEIGHT, WIDTH);
	cam.sqrt_spp = int(std::sqrt(sample_count));
	cam.recip_sqrt_spp = 1.0 / cam.sqrt_spp;

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
		SetCameraFocusValues(0.6, 6.0);
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
	case 7:
		SetCameraFocusValues(0, 6.0);
		cam.setCameraAngle(Point(278, 278, -800), Point(278, 278, 0), Point(0, 1, 0), 40.0);
		SetUpCornellSmoke();
		break;
	case 8:
		SetCameraFocusValues(0, 6.0);
		cam.setCameraAngle(Point(278, 278, -800), Point(278, 278, 0), Point(0, 1, 0), 40.0);
		FinalScene();
	case 9:
		SetCameraFocusValues(0, 10.0);
		//-3 6 9
		cam.setCameraAngle(Point(-3, 6, 9), Point(0, 0, 0), Point(0, 1, 0), 80.0);
		SetUpGltfScene();
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

Image::~Image()
{
	image.close();
}

void Image::PrintToFile()
{

	image << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";
	auto draw_start = std::chrono::steady_clock::now();

#define MT 1
#if MT
	std::for_each(std::execution::par, image_vertical_iterator.begin(), image_vertical_iterator.end(), [this](size_t j)
		{
			std::for_each(std::execution::par, image_horizontal_iterator.begin(), image_horizontal_iterator.end(), [this, &j](size_t i)
				{
					//std::clog << "Lines left = " << HEIGHT - j << "\n";
					Color pixel_color = Color(0.0f);
					for (int s_j = 0; s_j < cam.sqrt_spp; s_j++)
					{
						for (int s_i = 0; s_i < cam.sqrt_spp; s_i++)
						{
							auto ray = cam.GetRay(i, j, s_i, s_j);
							pixel_color += RayColor(ray, shapes, ray_depth,lights);
						}
					}
					/*
					for (size_t sample = 0; sample < sample_count; sample++)
					{
						auto ray = cam.GetRay(i, j);
						pixel_color += RayColor(ray, shapes, ray_depth);
					}
					*/
					colors[j * WIDTH + i] = pixel_color;
				});
		});
	WriteColorOnce(image, sample_count, colors);
#else
	for (size_t j = 0; j < HEIGHT; j++)
	{
		//std::clog << "Lines left = " << HEIGHT - j << "\n";
		for (size_t i = 0; i < WIDTH; i++)
		{
			Color pixel_color = Color(0.0f);
			for (size_t sample = 0; sample < sample_count; sample++)
			{
				auto ray = cam.GetRay(i, j);
			    pixel_color += RayColor(ray, shapes, ray_depth);
			}
			WriteColor(image, pixel_color, sample_count);
		}
	}
#endif
	auto draw_end = std::chrono::steady_clock::now();
	std::cout << "Total time taken: " << (draw_end - draw_start) / 1s <<" seconds" << std::endl;;
}

void Image::StoreColor(Color col, int x, int y)
{
	colors[y * WIDTH + x] = col;
}

void Image::SetUpSphereScene()
{
	auto checkerTex = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9));
	auto checker = std::make_shared<Lambertian>(checkerTex);
	shapes.Add(std::make_shared<Sphere>(Point(0, -10, -1.0), 10.0f, checker));
	shapes.Add(std::make_shared<Sphere>(Point(0, 10, -1.0), 10.0f, checker));
	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));

}

void Image::SetUpRandomBallScene()
{
	auto light = std::make_shared<DiffuseLight>(Color(50, 50, 50));
	shapes.Add(make_shared<Quad>(Point(123, 554, 147), Point(300, 0, 0), Point(0, 0, 265), light));

	auto checkerTex = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9));
	auto checker = std::make_shared<Lambertian>(checkerTex);

	shapes.Add(std::make_shared<Sphere>(Point(0, -1000, -1.0), 1000.0f, checker));
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
					shapes.Add(std::make_shared<Sphere>(center, center2, 0.2f, std::make_shared<Lambertian>(albedo)));
				}
				else if (choose_mat < 0.95)
				{
					auto albedo = RandomVector(0.5,1);
					auto fuzz = random_double(0,0.5);
					auto metal = std::make_shared<Metal>(albedo,fuzz);
					shapes.Add(std::make_shared<Sphere>(center, 0.2f, metal));
				}
				else
				{
					auto dielectric = std::make_shared<Dielectric>(1.5f);
					shapes.Add(std::make_shared<Sphere>(center, 0.2f, dielectric));
				}
			}
		}
	}
	shapes.Add(std::make_shared<Sphere>(Point(-4, 1, 0), 1.0f, checker));
	shapes.Add(std::make_shared<Sphere>(Point(0, 1, 0), 1.0f, std::make_shared<Dielectric>(1.5f)));
	shapes.Add(std::make_shared<Sphere>(Point(4, 1, 0), 1.0f, std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0f)));

	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
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
	
	shapes.Add(std::make_shared<Sphere>(Point(0), 2.0f, std::make_shared<Lambertian>(earthTex)));
	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
}

void Image::SetUpNoiseScene()
{
	auto NoiseTex = std::make_shared<NoiseTexture>(4);
	auto Noise = std::make_shared<Lambertian>(NoiseTex);
	shapes.Add(std::make_shared<Sphere>(Point(0, -1000, -1.0), 1000.0f, Noise));
	shapes.Add(std::make_shared<Sphere>(Point(0, 2, 0), 2.0f, Noise));

	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
}

void Image::SetUpQuads()
{ 
	auto leftRed = std::make_shared<Lambertian>(Color(1, 0, 0));
	auto backGreen = std::make_shared<Lambertian>(Color(0, 1, 0));
	auto rightBlue = std::make_shared<Lambertian>(Color(0, 0, 1));
	auto upperOrange = std::make_shared<Lambertian>(Color(1, 0.5, 0));
	auto lowerTeal = std::make_shared<Lambertian>(Color (0.2, 0.8, 0.8));
	auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));


	shapes.Add(std::make_shared<Quad>(Point(-3, -2, 5), Point(0, 0, -4), Point(0, 4, 0), leftRed));
	shapes.Add(std::make_shared<Quad>(Point(-2, -2, 0), Point(4, 0, 0), Point(0, 4, 0), backGreen));
	shapes.Add(std::make_shared<Quad>(Point(3, -2, 1), Point(0, 0, 4), Point(0, 4, 0), rightBlue));
	shapes.Add(std::make_shared<Quad>(Point(-2, 3, 1), Point(4, 0, 0), Point(0, 0, 4), upperOrange));
	shapes.Add(std::make_shared<Quad>(Point(-1, 3, 2.5), Point(2, 0, 0), Point(0, 0, -2), light));
	shapes.Add(std::make_shared<Quad>(Point(-2, -3, 5), Point(4, 0, 0), Point(0, 0, -4), lowerTeal));
	//shapes.Add(std::make_shared	<Triangle>(Point(-1, -1, 1), Point(1, -1, 1), Point(0, 1, 1), lowerTeal));


	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
}

void Image::SetUpCornellBox()
{
	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto light = std::make_shared<DiffuseLight>(Color(15,15,15));
	

	shapes.Add(std::make_shared<Quad>(Point(555, 0, 0), Point(0, 555, 0), Point(0, 0, 555), green));
	shapes.Add(std::make_shared<Quad>(Point(0, 0, 0), Point(0, 555, 0), Point(0, 0, 555), red));
	shapes.Add(std::make_shared<Quad>(Point(343, 554, 332), Point(-130, 0, 0), Point(0, 0, -105), light));
	shapes.Add(std::make_shared<Quad>(Point(0, 0, 0), Point(555, 0, 0), Point(0, 0, 555), white));
	shapes.Add(std::make_shared<Quad>(Point(555, 555, 555), Point(-555, 0, 0), Point(0, 0, -555), white));
	shapes.Add(std::make_shared<Quad>(Point(0, 0, 555), Point(555, 0, 0), Point(0, 555, 0), white));

	std::shared_ptr<Shape> box1 = Box(Point(0), Point(165, 330, 165), white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, Point(265, 0, 295));
	shapes.Add(box1);

	std::shared_ptr<Shape> box2 = Box(Point(0), Point(165, 165, 165), white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, Point(130, 0, 65));
	shapes.Add(box2);
	
	//Light setup'
	auto empty_material = std::make_shared<Material>();
	lights.Add(make_shared<Quad>(Point(343, 554, 332), Point(-130, 0, 0), Point(0, 0, -105), empty_material));
	//lights.Add(std::make_shared<Sphere>(Point(190, 90, 190), 90, empty_material));

	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
}

void Image::SetUpCornellSmoke()
{
	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));


	shapes.Add(std::make_shared<Quad>(Point(555, 0, 0), Point(0, 555, 0), Point(0, 0, 555), green));
	shapes.Add(std::make_shared<Quad>(Point(0, 0, 0), Point(0, 555, 0), Point(0, 0, 555), red));
	shapes.Add(std::make_shared<Quad>(Point(113, 554, 127), Point(330, 0, 0), Point(0, 0, 305), light));
	shapes.Add(std::make_shared<Quad>(Point(0, 0, 0), Point(555, 0, 0), Point(0, 0, 555), white));
	shapes.Add(std::make_shared<Quad>(Point(555, 555, 555), Point(-555, 0, 0), Point(0, 0, -555), white));
	shapes.Add(std::make_shared<Quad>(Point(0, 0, 555), Point(555, 0, 0), Point(0, 555, 0), white));
  
	//shapes.Add(std::make_shared	<Triangle>(Point(345, 276, 450), Point(414, 138, 450), Point(276, 138, 450),green));
	
	std::shared_ptr<Shape> box1 = Box(Point(0), Point(165, 330, 165), white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, Point(265,0,295));

	std::shared_ptr<Shape> box2 = Box(Point(0), Point(165, 165, 165), white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, Point(130,0,65));

	auto empty_material = std::make_shared<Material>();
	lights.Add(make_shared<Quad>(Point(343, 554, 332), Point(-130, 0, 0), Point(0, 0, -105), empty_material));
	lights.Add(std::make_shared<Sphere>(Point(190, 90, 190), 90, empty_material));


	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
}

void Image::SetUpGltfScene()
{
	auto leftRed = std::make_shared<Lambertian>(Color(1, 0, 0));

	auto backGreen = std::make_shared<Lambertian>(Color(0, 1, 0));
	auto rightBlue = std::make_shared<Lambertian>(Color(0, 0, 1));
	auto upperOrange = std::make_shared<Lambertian>(Color(1, 0.5, 0));
	auto lowerTeal = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));
	auto col = std::make_shared<Lambertian>(Color(0.65, 0.65, 0.05));
	auto glass = std::make_shared<Dielectric>(1.5f);
	auto metal = std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 0.5);
	auto teal = lowerTeal;
	auto light = std::make_shared<DiffuseLight>(Color(15));
	auto purple = std::make_shared<Lambertian>(Color(0.521, 0.349, 0.533));

	auto checkerTex = std::make_shared<CheckerTexture>(0.32, Color(0, 1, 0), Color(0.2, 0.8, 0.8));
	auto checker = std::make_shared<Lambertian>(checkerTex);
	/*
	//left
	shapes.Add(std::make_shared<Quad>(Point(-4, -4, 5), Point(0, 0, -8), Point(0, 8, 0), leftRed));
	//back
	shapes.Add(std::make_shared<Quad>(Point(-4, -4, 1), Point(8, 0, 0), Point(0, 8, 0),backGreen));
	//right
	shapes.Add(std::make_shared<Quad>(Point(4, -4, 1), Point(0, 0, 8), Point(0, 8, 0), teal));
	//top
	shapes.Add(std::make_shared<Quad>(Point(-4, 4.0001, 5), Point(8, 0, 0), Point(0, 0, -8), purple));
	//bottom
	shapes.Add(std::make_shared<Quad>(Point(-4, -4, 5), Point(8, 0, 0), Point(0, 0, -8), lowerTeal));
	//background
	shapes.Add(std::make_shared<Quad>(Point(-50, -50, -10), Point(100, 0, 0), Point(0, 100, 0), purple));
	//front
	shapes.Add(std::make_shared<Quad>(Point(-4, -4, 10), Point(8, 0, 0), Point(0, 8, 0), backGreen));

	//shapes.Add(std::make_shared<Quad>(Point(-1, -2, 4), Point(2, 0, 0), Point(0, 0, -2), light));
	//shapes.Add(std::make_shared<Sphere>(Point(0, 130, 0), 30.0f, light));
	Mesh model("assets/monkey.glb", Point(0,-0.8,4));
	model.AddToScene(shapes);
	*/

	for (int a = -22; a < 22; a++)
	{
		for (int b = -22; b < 22; b++)
		{
			double choose_mat = random_double();
			Point center = Point(a + 0.9f * random_double(), -3.0f, b + 0.9f * random_double());

			if (glm::length(center - Point(4, 0.2, 0)) > 0.9f)
			{
				if (choose_mat < 0.1f)
				{
					shapes.Add(std::make_shared<Sphere>(Point(center), 0.2f, light));
				}
				if (choose_mat < 0.8f)
				{
					auto albedo = RandomVector() * RandomVector();
					auto center2 = center + Point(0, random_double(0, .5), 0);
					shapes.Add(std::make_shared<Sphere>(center, center2, 0.2f, std::make_shared<Lambertian>(albedo)));
				}
				else if (choose_mat < 0.95)
				{
					auto albedo = RandomVector(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					auto metal = std::make_shared<Metal>(albedo, fuzz);
					shapes.Add(std::make_shared<Sphere>(center, 0.2f, metal));
				}
				else
				{
					auto dielectric = std::make_shared<Dielectric>(1.5f);
					shapes.Add(std::make_shared<Sphere>(center, 0.2f, dielectric));
				}
			}
		}
	}

	shapes.Add(std::make_shared<Quad>(Point(-1.5, 8, 2), Point(3, 0, 0), Point(0, 0, 2), light));
	//shapes.Add(std::make_shared<Sphere>(Point(-0.5f, -3, 3.5f), 0.3f, light));
	//shapes.Add(std::make_shared<Sphere>(Point(0.5f, -3, 4.5f), 0.3f, light));
	//shapes.Add(std::make_shared<Sphere>(Point(-0.5f, -3, 3.5f), 0.3f, light));

	shapes.Add(std::make_shared<Sphere>(Point(-10, 15,25), 10.0f, light));

	shapes.Add(std::make_shared<Quad>(Point(-40, -6, 40), Point(80, 0, 0), Point(0, 0, -80), checker));

	Mesh model("assets/dragon3.glb", Point(0, -1.5, 4));
	model.AddToScene(shapes, metal);

	model.SetWorldPos(Point(-3, 1.5, 2.0f));
	model.AddToScene(shapes, glass);

	model.SetWorldPos(Point(3, 1.5, 2));
	model.AddToScene(shapes, leftRed);
	auto empty_material = std::make_shared<Material>();
	lights.Add(make_shared<Quad>(Point(-1.5, 8, 2), Point(3, 0, 0), Point(0, 0, 2), empty_material));
	lights.Add(std::make_shared<Sphere>(Point(-10, 15, 25), 10.0f, empty_material));


	//lights.Add(std::make_shared<Sphere>(Point(0, -1, 0), 1.0f, empty_material));
	//bottom	

	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
}

void Image::FinalScene()
{
	auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

	ShapeList boxes1;
	int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101);
			auto z1 = z0 + w;

			boxes1.Add(Box(Point(x0, y0, z0), Point(x1, y1, z1), ground));
		}
	}

	shapes.Add(std::make_shared<BVH_Node>(boxes1));

	auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
	shapes.Add(make_shared<Quad>(Point(123, 554, 147), Point(300, 0, 0), Point(0, 0, 265), light));

	auto center1 = Point(400, 400, 200);
	auto center2 = center1 + glm::vec3(30, 0, 0);
	auto sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
	shapes.Add(std::make_shared<Sphere>(center1, center2, 50, sphere_material));

	shapes.Add(std::make_shared<Sphere>(Point(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
	shapes.Add(std::make_shared<Sphere>(Point(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

	auto boundary = std::make_shared<Sphere>(Point(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
	shapes.Add(boundary);
	shapes.Add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
	boundary = std::make_shared<Sphere>(Point(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
	shapes.Add(std::make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

	auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
	shapes.Add(std::make_shared<Sphere>(Point(400, 200, 400), 100, emat));
	auto pertext = std::make_shared<NoiseTexture>(0.1);
	shapes.Add(std::make_shared<Sphere>(Point(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

	ShapeList boxes2;
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.Add(std::make_shared<Sphere>(RandomVector(0, 165), 10, white));
	}

	shapes.Add(std::make_shared<Translate>(make_shared<RotateY>(std::make_shared<BVH_Node>(boxes2), 15),
		glm::vec3(-100, 270, 395)));

	shapes = ShapeList(std::make_shared<BVH_Node>(shapes));
}