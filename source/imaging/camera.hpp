#pragma once
#include "../math/point3.hpp"
#include "../math/vec3.hpp"
#include "../math/ray.hpp"

class Camera
{
public:

    Camera() = default;
    ~Camera() = default;

    Camera(float v_fov, float aspect);
    Camera(const Point3f& look_from, const Point3f& look_at, const Vec3f& v_up, const float v_fov, const float aspect);

    Rayf GetRay(const float u, const float v) const;

private:
    Point3f m_origin;
    Point3f m_lowerLeftCorner;
    Vec3f m_horizontal;
    Vec3f m_vertical;
};
