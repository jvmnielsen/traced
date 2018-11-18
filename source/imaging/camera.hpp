#pragma once
#include "../math/point3.hpp"
#include "../math/vec3.hpp"
#include "../math/ray.hpp"

class camera
{
public:

    camera() = default;
    ~camera() = default;

    camera(float v_fov, float aspect);
    camera(const Point3f& look_from, const Point3f& look_at, const Vec3f& v_up, const float v_fov, const float aspect);

    Rayf GetRay(const float u, const float v) const;

private:
    Point3f m_origin;
    Point3f m_lowerLeftCorner;
    Vec3f m_horizontal;
    Vec3f m_vertical;
};
