#include "Camera.h"
#include <math.h>

Camera::Camera(const float v_fov, const float aspect)
{
    const auto theta = v_fov * M_PI / 180; // degrees to radians
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;
    lowerLeftCorner = Vecf(-half_width, -half_height, -1.0);
    horizontal = Vecf(2 * half_width, 0.0f, 0.0f);
    vertical = Vecf(0.0f, 2 * half_height, 0.0f);
    origin = Vecf(0.0f);
}

Camera::Camera(
    const Vecf& look_from, 
    const Vecf& look_at, 
    const Vecf& v_up, 
    const float v_fov, 
    const float aspect)
{
    const auto theta = v_fov * M_PI / 180;
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;

    origin = look_from;
    
    const auto w = UnitVector(look_from - look_at);
    const auto u = UnitVector(CrossProduct(v_up, w));
    const auto v = CrossProduct(w, u);

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
	} while (DotProduct(p, p) >= 1.0);
	return p;
} */

