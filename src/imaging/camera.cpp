#include "camera.hpp"
#include "../core/sampler.hpp"

using namespace tr;
using namespace gm;

Camera::Camera(FLOAT v_fov, FLOAT aspect)
    : m_u(Vec3f{1,0,0}.normalise()),
      m_v(Vec3f{0,1,0}.normalise()),
      m_w(Vec3f{0,0,1}.normalise()) {

    auto const theta = v_fov * gm::constants::pi / 180; // degrees to radians
    auto const half_height = static_cast<float>(tan(theta / 2));
    auto const half_width = aspect * half_height;
    m_lower_left_corner = gm::Point3f(-half_width, -half_height, -1.0f);
    m_horizontal = gm::Vec3f(2.0f * half_width, 0.0f, 0.0f);
    m_vertical = gm::Vec3f(0.0f, 2.0f * half_height, 0.0f);
    m_origin = gm::Point3f(0.0f);
}

Camera::Camera(
        gm::Point3f const& look_from,
        gm::Point3f const& look_at,
        FLOAT v_fov,
        FLOAT aspect,
        FLOAT aperture,
        FLOAT focus_dist)
    : m_lens_radius(aperture / 2),
      m_u(Vec3f{1,0,0}.normalise()),
      m_v(Vec3f{0,1,0}.normalise()),
      m_w(Vec3f{0,0,1}.normalise()) {

    auto const theta = gm::degree_to_radian(v_fov);
    auto const half_height = tan(theta / 2);
    auto const half_width = aspect * half_height;

    m_origin = look_from;
    auto const v_up = gm::Vec3f{0.0f, 1.0f, 0.0f};

    m_w = (look_from - look_at).normalise();
    m_u = (gm::cross(v_up, m_w)).normalise();
    m_v = (gm::cross(m_w, m_u)).normalise();

    m_lower_left_corner = m_origin - half_width * focus_dist * m_u - half_height * focus_dist * m_v - focus_dist * m_w;
    m_horizontal = 2 * half_width * focus_dist * m_u;
    m_vertical = 2 * half_height * focus_dist * m_v;
}

auto Camera::get_ray(FLOAT s, FLOAT t, Sampler& sampler) const -> Rayf {

    auto const rd = m_lens_radius * sampler.sample_disk();
    auto const offset = rd.x * m_u + rd.y * m_v;

    return {m_origin + offset, // origin of the camera
            m_lower_left_corner + m_horizontal * s + m_vertical * t - m_origin - offset}; // scale from lower left - origin for vector pointing to this point
}