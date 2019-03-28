#include "transform.hpp"
#include "math_util.hpp"


auto
Transform::Translate(const Vec3f& vec) -> Transform& {
    
    
    m_matrix *= {
        1, 0, 0, vec.x(),
        0, 1, 0, vec.y(),
        0, 0, 1, vec.z(),
        0, 0, 0,     1
    };
    m_inverse *= {
        1, 0, 0, -vec.x(),
        0, 1, 0, -vec.y(),
        0, 0, 1, -vec.z(),
        0, 0, 0,      1
    };

    return *this;
}

auto 
Transform::Scale(const Vec3f& vec) -> Transform&
{
    m_matrix *= {
        vec.x(),     0,     0, 0,
            0, vec.y(),     0, 0,
            0,     0, vec.z(), 0,
            0,     0,     0,   1
    };

    m_inverse *= {
        1.0f/vec.x(),          0,         0,  0,
        0,          1.0f/vec.y(),         0,  0,
        0,                  0, 1.0f/vec.z(),  0,
        0,                  0,         0,  1
    };

    return *this;
}

auto
Transform::Rotate(const Vec3f& axis, float angle) -> Transform& {

    const auto norm_axis = normalize(axis);
    const auto rad = Math::DegreeToRadian(angle);

    const auto cos_theta = std::cos(rad);
    const auto sin_theta = std::sin(rad);

    Matrix4x4f mat;

    mat(0, 0) = norm_axis.x() * norm_axis.x() + (1.0f - norm_axis.x() * norm_axis.x()) * cos_theta;
    mat(0, 1) = norm_axis.x() * norm_axis.y() * (1.0f - cos_theta) - norm_axis.z() * sin_theta;
    mat(0, 2) = norm_axis.x() * norm_axis.z() * (1.0f - cos_theta) + norm_axis.y() * sin_theta;
    mat(0, 3) = 0;

    mat(1, 0) = norm_axis.x() * norm_axis.y() * (1.0f - cos_theta) + norm_axis.z() * sin_theta;
    mat(1, 1) = norm_axis.y() * norm_axis.y() + (1.0f - norm_axis.y() * norm_axis.y()) * cos_theta;
    mat(1, 2) = norm_axis.y() * norm_axis.z() * (1.0f - cos_theta) - norm_axis.x() * sin_theta;
    mat(1, 3) = 0;

    mat(2, 0) = norm_axis.x() * norm_axis.z() * (1.0f - cos_theta) - norm_axis.y() * sin_theta;
    mat(2, 1) = norm_axis.y() * norm_axis.z() * (1.0f - cos_theta) + norm_axis.x() * sin_theta;
    mat(2, 2) = norm_axis.z() * norm_axis.z() + (1.0f - norm_axis.z() * norm_axis.z()) * cos_theta;
    mat(2, 3) = 0;

    mat(3, 0) = 0;
    mat(3, 1) = 0;
    mat(3, 2) = 0;
    mat(3, 3) = 1;

    m_matrix *= mat;
    m_inverse *= mat.Transpose();

    return *this;
}

/*
Point3f Transform::TransformAffine(const Point3f& point) const
{
    const auto x = m_matrix(0,0) * point.x + m_matrix(0,1) * point.y
                 + m_matrix(0,2) * point.z + m_matrix(0,3);
    const auto y = m_matrix(1,0) * point.x + m_matrix(1,1) * point.y
                 + m_matrix(1,2) * point.z + m_matrix(1,3);
    const auto z = m_matrix(2,0) * point.x + m_matrix(2,1) * point.y
                 + m_matrix(2,2) * point.z + m_matrix(2,3);
    return { x, y, z};
} */


auto
Transform::apply_to_point(const Point3f& point) const -> Point3f {

    const auto x = m_matrix(0,0) * point.x() + m_matrix(0,1) * point.y() + m_matrix(0,2) * point.z() + m_matrix(0,3);
    const auto y = m_matrix(1,0) * point.x() + m_matrix(1,1) * point.y() + m_matrix(1,2) * point.z() + m_matrix(1,3);
    const auto z = m_matrix(2,0) * point.x() + m_matrix(2,1) * point.y() + m_matrix(2,2) * point.z() + m_matrix(2,3);
    const auto w = m_matrix(3,0) * point.x() + m_matrix(3,1) * point.y() + m_matrix(3,2) * point.z() + m_matrix(3,3);

    return {x/w, y/w, z/w};
}

auto
Transform::apply_to_vector(const Vec3f& vec) const -> Vec3f {

    const auto x = m_matrix(0,0) * vec.x() + m_matrix(0,1) * vec.y() + m_matrix(0,2) * vec.z();
    const auto y = m_matrix(1,0) * vec.x() + m_matrix(1,1) * vec.y() + m_matrix(1,2) * vec.z();
    const auto z = m_matrix(2,0) * vec.x() + m_matrix(2,1) * vec.y() + m_matrix(2,2) * vec.z();
    return {x, y, z};
}

auto
Transform::apply_to_normal(const Vec3f& normal) const -> Vec3f {

    // Note indices: we're using the transpose
    const auto x = m_inverse(0,0) * normal.x() + m_inverse(1,0) * normal.y() + m_inverse(2,0) * normal.z();
    const auto y = m_inverse(0,1) * normal.x() + m_inverse(1,1) * normal.y() + m_inverse(2,1) * normal.z();
    const auto z = m_inverse(0,2) * normal.x() + m_inverse(1,2) * normal.y() + m_inverse(2,2) * normal.z();
    return {x, y, z};
}

/*
auto
Transform::Inverse(const Point3f& point) const -> Point3f {

    const auto x = m_inverse(0,0) * point.x() + m_inverse(0,1) * point.y() + m_inverse(0,2) * point.z() + m_inverse(0,3);
    const auto y = m_inverse(1,0) * point.x() + m_inverse(1,1) * point.y() + m_inverse(1,2) * point.z() + m_inverse(1,3);
    const auto z = m_inverse(2,0) * point.x() + m_inverse(2,1) * point.y() + m_inverse(2,2) * point.z() + m_inverse(2,3);
    const auto w = m_inverse(3,0) * point.x() + m_inverse(3,1) * point.y() + m_inverse(3,2) * point.z() + m_inverse(3,3);

    return {x/w, y/w, z/w};
} */



