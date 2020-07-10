#pragma once

#include "matrix4x4.hpp"
#include "vec3.hpp"
#include "point3.hpp"
#include "normal3.hpp"

class Transform
{
public:
    Transform() = default;

    auto translate(const Vec3f& vec) -> Transform&;
    auto scale(const Vec3f& vec) -> Transform&;
    auto rotate(const Vec3f& axis, float angle) -> Transform&;

    auto apply_to_point(const Point3f& point) const -> Point3f;
    auto apply_to_vector(const Vec3f& vec) const -> Vec3f;
    auto apply_to_normal(const Vec3f& normal) const -> Vec3f;

private:
    Matrix4x4f m_matrix;
    Matrix4x4f m_inverse;
};
