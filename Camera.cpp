#include "Camera.h"
#include <SDL_stdinc.h>

Camera::Camera(const float v_fov, const float aspect)
{
    const auto theta = v_fov * M_PI / 180; // degrees to radians
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;
    m_lower_left_corner = Vec3f(-half_width, -half_height, -1.0);
    m_horizontal = Vec3f(2 * half_width, 0.0f, 0.0f);
    m_vertical = Vec3f(0.0f, 2 * half_height, 0.0f);
    m_origin = Vec3f(0.0f);
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

    m_origin = look_from;
    
    const auto w = unit_vector(look_from - look_at);
    const auto u = unit_vector(cross(v_up, w));
    const auto v = cross(w, u);

    m_lower_left_corner = m_origin - half_width * u - half_height * v - w;
    m_horizontal = 2 * half_width * u;
    m_vertical = 2 * half_height * v;
}

Rayf Camera::get_ray(const float u, const float v) const
{
    return {m_origin, 
        m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin};
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

