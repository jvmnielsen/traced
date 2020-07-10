#pragma once
#include "../math/point3.hpp"
#include "../math/vec3.hpp"
#include "../math/ray.hpp"

class Sampler;

class Camera
{
public:

    Camera(float v_fov, float aspect);
    Camera(const Point3f& look_from, const Point3f& look_at, FLOAT v_fov, FLOAT aspect, FLOAT aperture, FLOAT focus_dist);

    auto get_ray(double s, double t, Sampler& sampler) const -> Rayf;

private:
    Point3f m_origin;
    Point3f m_lower_left_corner;
    Vec3f m_horizontal;
    Vec3f m_vertical;
    Vec3f m_u, m_v, m_w;
    FLOAT m_lens_radius;
};
