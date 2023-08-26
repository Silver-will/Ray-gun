#include "Camera.h"

Camera::Camera()
{

}

glm::vec3 Camera::GetCameraOrigin()
{
	return center;
}

glm::vec3 Camera::GetPixelCenter(size_t& row, size_t& col)
{
	auto r = static_cast<float>(row);
	auto c = static_cast<float>(col);
	glm::vec3 pixel_center = pixel100_loc + (r * pixel_delta_u) + (c * pixel_delta_v);
	return pixel_center;
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