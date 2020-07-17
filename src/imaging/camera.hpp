#pragma once

#include "../geometry/ray.hpp"

#include <graphics-math.hpp>

namespace tr {

    class Sampler;

    class Camera
    {
    public:

        Camera(float v_fov, float aspect);
        Camera(const gm::Point3f& look_from, const gm::Point3f& look_at, FLOAT v_fov, FLOAT aspect, FLOAT aperture, FLOAT focus_dist);

        auto get_ray(FLOAT s, FLOAT t, Sampler& sampler) const -> Rayf;

    private:
        gm::Point3f m_origin;
        gm::Point3f m_lower_left_corner;
        gm::Vec3f m_horizontal;
        gm::Vec3f m_vertical;
        gm::Normal3f m_u, m_v, m_w;
        FLOAT m_lens_radius;
    };

}
