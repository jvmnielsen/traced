#include "camera.hpp"
#include "../math/math_util.hpp"
#include "../math/normal3.hpp"

Camera::Camera(const float v_fov, const float aspect)
{
    const auto theta = v_fov * Math::Constants::Pi / 180; // degrees to radians
    const auto half_height = static_cast<float>(tan(theta / 2));
    const auto half_width = aspect * half_height;
    m_lower_left_corner = Point3f(-half_width, -half_height, -1.0f);
    m_horizontal = Vec3f(2.0f * half_width, 0.0f, 0.0f);
    m_vertical = Vec3f(0.0f, 2.0f * half_height, 0.0f);
    m_origin = Point3f(0.0f);
}

Camera::Camera(
        const Point3f& look_from,
        const Point3f& look_at,
        const Vec3f& v_up,
        const float v_fov,
        const float aspect)
{
    const auto theta = Math::DegreeToRadian(v_fov);
    const auto halfHeight = tan(theta / 2);
    const auto halfWidth = aspect * halfHeight;

    m_origin = look_from;

    const auto w = normalize(look_from - look_at);
    const auto u = normalize(cross(v_up, w));
    const auto v = normalize(cross(w, u));

    m_lower_left_corner = m_origin - u * halfWidth - v * halfHeight - w;
    m_horizontal = u * 2 * halfWidth;
    m_vertical = v * 2 * halfHeight;
}

Rayf Camera::get_ray(double u, double v) const
{
    return {m_origin, // origin of the camera
            m_lower_left_corner + m_horizontal * u + m_vertical * v - m_origin}; // scale from lower left - origin for vector pointing to this point

}