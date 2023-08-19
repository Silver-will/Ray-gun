#ifndef CAMERA_H
#define CAMERA_H
#include<glm/glm.hpp>

struct Camera
{
	Camera(uint16_t& height, uint16_t& width);
	Camera();
	void generate_viewport_variables(auto h, auto w);
	glm::vec3 GetPixelCenter(size_t& row, size_t& col);
	glm::vec3 GetCameraOrigin();
private:
	glm::vec3 center;
	float viewport_height;
	float viewport_width;
	float focal_length;
	glm::vec3 viewport_u;
	glm::vec3 viewport_v;
	glm::vec3 pixel_delta_u;
	glm::vec3 pixel_delta_v;
	glm::vec3 viewport_upper_left;
	glm::vec3 pixel100_loc;
};
#endif
