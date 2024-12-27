#include "Camera.h"
#include "Common.h"
Camera::Camera()
{

}

glm::vec3 Camera::GetCameraOrigin() const 
{
	return center;
}

Ray Camera::GetRay(size_t& row, size_t& col, const size_t& s_i ,const size_t& s_j ) const
{
	auto r = static_cast<float>(row);
	auto c = static_cast<float>(col);
	glm::vec3 pixel_center = pixel100_loc + (r * pixel_delta_u) + (c * pixel_delta_v);
	auto pixel_sample = pixel_center + PixelSampleSquare(s_i, s_j);
	auto ray_origin = defocus_angle <= 0 ? center : DefocusDiskSample();
	auto ray_direction = pixel_sample - ray_origin;
	auto ray_time = random_double();


	return Ray(ray_origin, ray_direction,ray_time);
}
Camera::Camera(uint16_t& height, uint16_t& width):image_height{height}, image_width{width}
{
	look_from = Point(0, 0, -1);
	look_at = Point(0, 0, 0);
	v_up = glm::vec3(0, 1, 0);
	sample_count = 10;
	max_depth = 5;
}

//Generate viewport
void Camera::generate_viewport_variables(auto h, auto w)
{
	center = look_from;

	auto theta = degrees_to_radians(vfov);
	auto hi = tan(theta / 2);
	viewport_height = 2.0f * hi * focus_dist;
	viewport_width = viewport_height * (static_cast<float>(w) / h);

	w_ = glm::normalize(look_from - look_at);
	u_ = glm::normalize(glm::cross(v_up, w_));
	v_ = glm::cross(w_, u_);

	viewport_u = viewport_width * u_;
	viewport_v = viewport_height * -v_;

	pixel_delta_u = viewport_u / (float)w;
	pixel_delta_v = viewport_v / (float)h;

	viewport_upper_left = center - (focus_dist * w_) - (viewport_u / 2.0f) - (viewport_v / 2.0f);

	float defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
	defocus_disk_u = u_ * defocus_radius;
	defocus_disk_v = v_ * defocus_radius;
	pixel100_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

glm::vec3 Camera::PixelSampleSquare(const double& s_i, const double& s_j) const
{
	float px = -0.5 + recip_sqrt_spp * (s_i + random_double());
	float py = -0.5 + recip_sqrt_spp * (s_j + random_double());
	return glm::vec3((px * pixel_delta_u) + (py * pixel_delta_v));
}

void Camera::setCameraAngle(Point look_f, Point look_a,Point vup, double fov)
{
	look_from = look_f;
	look_at = look_a;
	vfov = fov;
	v_up = vup;
	generate_viewport_variables(image_height, image_width);
}

Point Camera::DefocusDiskSample() const
{
	auto p = RandomInUnitDisk();
	return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}