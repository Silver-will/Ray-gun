#include "Camera.h"
#include "Common.h"
Camera::Camera()
{
	sample_count = 10;
	max_depth = 5;
}

glm::vec3 Camera::GetCameraOrigin() const 
{
	return center;
}

Ray Camera::GetRay(size_t& row, size_t& col) const
{
	auto r = static_cast<float>(row);
	auto c = static_cast<float>(col);
	glm::vec3 pixel_center = pixel100_loc + (r * pixel_delta_u) + (c * pixel_delta_v);
	auto pixel_sample = pixel_center + PixelSampleSquare();
	auto ray_origin = center;
	auto ray_direction = pixel_sample - ray_origin;

	return Ray(ray_origin, ray_direction);
}
Camera::Camera(uint16_t& height, uint16_t& width) :center{glm::vec3(0.0)}
{
	generate_viewport_variables(height, width);
}

//Generate viewport
void Camera::generate_viewport_variables(auto h, auto w)
{
	focal_length = 1.0f;
	viewport_height = 2.0f;
	viewport_width = viewport_height * (static_cast<float>(w) / h);

	viewport_u = glm::vec3(viewport_width, 0, 0);
	viewport_v = glm::vec3(0, -viewport_height, 0);

	pixel_delta_u = viewport_u / (float)w;
	pixel_delta_v = viewport_v / (float)h;

	viewport_upper_left = center - glm::vec3(0, 0, focal_length) - (viewport_u / 2.0f) - (viewport_v / 2.0f);

	pixel100_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

}

glm::vec3 Camera::PixelSampleSquare() const
{
	float px = -0.5 * random_double();
	float py = -0.5 * random_double();
	return glm::vec3((px * pixel_delta_u) + (py * pixel_delta_v));
}