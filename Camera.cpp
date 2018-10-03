#include "Camera.h"
#include <SDL_stdinc.h>

Camera::Camera(const float v_fov, const float aspect)
{
    const auto theta = v_fov * M_PI / 180; // degrees to radians
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;
    lowerLeftCorner = Vec3f(-half_width, -half_height, -1.0);
    horizontal = Vec3f(2 * half_width, 0.0f, 0.0f);
    vertical = Vec3f(0.0f, 2 * half_height, 0.0f);
    origin = Vec3f(0.0f);
}

Camera::Camera(
    const Vec3f& look_from, 
    const Vec3f& look_at, 
    const Vec3f& v_up, 
    const float v_fov, 
    const float aspect)
{
    const auto theta = v_fov * M_PI / 180;
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;

    origin = look_from;
    
    const auto w = unit_vector(look_from - look_at);
    const auto u = unit_vector(cross(v_up, w));
    const auto v = cross(w, u);

    lowerLeftCorner = origin - half_width * u - half_height * v - w;
    horizontal = 2 * half_width * u;
    vertical = 2 * half_height * v;
}

Rayf Camera::GetRay(const float u, const float v) const
{
    return {origin, 
        lowerLeftCorner + u * horizontal + v * vertical - origin};
}

/*
Vec3f Camera::random_in_unit_disk()
{
    Vec3f p;
	do {
		p = 2.0 * Vec3f( m_dist( m_gen ), m_dist( m_gen ), 0) - Vec3f(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
} */

