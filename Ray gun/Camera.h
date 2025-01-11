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
	Ray GetRay(int i, int j, int s_i, int s_j);
	glm::vec3 GetCameraOrigin()const;
	glm::vec3 PixelSampleSquare() const;
	glm::vec3 PixelSampleSquare(int s_i, int s_j) const;
	Point DefocusDiskSample()const;
	void setCameraAngle(Point look_f = Point(0, 0, -1), Point look_a = Point(0), Point vup = Point(0, 1, 0),
		double fov = 90);
	float defocus_angle = 0;
	float focus_dist = 10;
	glm::vec3 GetPixelCenter(size_t& row, size_t& col);
	glm::vec3 GetCameraOrigin();
	int sqrt_spp;
	double recip_sqrt_spp;
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
	Point look_at;
	Point look_from;
	glm::vec3 v_up;
	glm::vec3 u_, v_, w_;
	glm::vec3 defocus_disk_u;
	glm::vec3 defocus_disk_v;
	double vfov = 90;
};
#endif
