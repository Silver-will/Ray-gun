#ifndef CAMERA_H
#define CAMERA_H
#include<glm/glm.hpp>
#include "Ray.h"

struct Camera
{
	Camera(uint16_t& height, uint16_t& width);
	Camera();
	void generate_viewport_variables(auto h, auto w);
	Ray GetRay(size_t& row, size_t& col)const;
	glm::vec3 GetCameraOrigin()const;
	glm::vec3 PixelSampleSquare() const;
private:
	uint16_t sample_count;
	uint16_t max_depth;
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
