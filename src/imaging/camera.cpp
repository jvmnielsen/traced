#include "camera.hpp"
#include "../core/sampler.hpp"

Camera::Camera(const float v_fov, const float aspect)
{
    const auto theta = v_fov * gm::constants::pi / 180; // degrees to radians
    const auto half_height = static_cast<float>(tan(theta / 2));
    const auto half_width = aspect * half_height;
    m_lower_left_corner = gm::Point3f(-half_width, -half_height, -1.0f);
    m_horizontal = gm::Vec3f(2.0f * half_width, 0.0f, 0.0f);
    m_vertical = gm::Vec3f(0.0f, 2.0f * half_height, 0.0f);
    m_origin = gm::Point3f(0.0f);
}

Camera::Camera(
        const gm::Point3f& look_from,
        const gm::Point3f& look_at,
        FLOAT v_fov,
        FLOAT aspect,
        FLOAT aperture,
        FLOAT focus_dist)
    : m_lens_radius(aperture / 2)
{
    const auto theta = gm::degree_to_radian(v_fov);
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;

    m_origin = look_from;
    const auto v_up = gm::Vec3f{0.0f, 1.0f, 0.0f};

    m_w = (look_from - look_at).to_normal();
    m_u = (gm::cross(v_up, m_w)).to_normal();
    m_v = (gm::cross(m_w, m_u)).to_normal();

    m_lower_left_corner = m_origin - m_u * half_width * focus_dist - m_v * half_height * focus_dist - m_w * focus_dist;
    m_horizontal = 2 * half_width * focus_dist * m_u;
    m_vertical = 2 * half_height * focus_dist * m_v;

    
}

auto Camera::get_ray(double s, double t, Sampler& sampler) const -> Rayf
{
    const auto rd = m_lens_radius * sampler.sample_disk();
    const auto offset = m_u * rd.x + m_v * rd.y;

    return {m_origin + offset, // origin of the camera
            normalize(m_lower_left_corner + m_horizontal * s + m_vertical * t - m_origin - offset)}; // scale from lower left - origin for vector pointing to this point

}