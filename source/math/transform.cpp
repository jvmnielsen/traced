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
        1.0/vec.x(),          0,         0,  0,
        0,          1.0/vec.y(),         0,  0,
        0,                  0, 1.0/vec.z(),  0,
        0,                  0,         0,  1
    };

    return *this;
}

auto
Transform::Rotate(const Vec3f& axis, float angle) -> Transform& {

    const auto normAxis = Normal3f{axis};
    const auto rad = Math::DegreeToRadian(angle);

    const auto cosTheta = std::cos(rad);
    const auto sinTheta = std::sin(rad);

    Matrix4x4f mat;

    mat(0, 0) = normAxis.x() * normAxis.x() + (1.0f - normAxis.x() * normAxis.x()) * cosTheta;
    mat(0, 1) = normAxis.x() * normAxis.y() * (1.0f - cosTheta) - normAxis.z() * sinTheta;
    mat(0, 2) = normAxis.x() * normAxis.z() * (1.0f - cosTheta) + normAxis.y() * sinTheta;
    mat(0, 3) = 0;

    mat(1, 0) = normAxis.x() * normAxis.y() * (1.0f - cosTheta) + normAxis.z() * sinTheta;
    mat(1, 1) = normAxis.y() * normAxis.y() + (1.0f - normAxis.y() * normAxis.y()) * cosTheta;
    mat(1, 2) = normAxis.y() * normAxis.z() * (1.0f - cosTheta) - normAxis.x() * sinTheta;
    mat(1, 3) = 0;

    mat(2, 0) = normAxis.x() * normAxis.z() * (1.0f - cosTheta) - normAxis.y() * sinTheta;
    mat(2, 1) = normAxis.y() * normAxis.z() * (1.0f - cosTheta) + normAxis.x() * sinTheta;
    mat(2, 2) = normAxis.z() * normAxis.z() + (1.0f - normAxis.z() * normAxis.z()) * cosTheta;
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
Transform::operator()(const Point3f& point) const -> Point3f {

    const auto x = m_matrix(0,0) * point.x() + m_matrix(0,1) * point.y() + m_matrix(0,2) * point.z() + m_matrix(0,3);
    const auto y = m_matrix(1,0) * point.x() + m_matrix(1,1) * point.y() + m_matrix(1,2) * point.z() + m_matrix(1,3);
    const auto z = m_matrix(2,0) * point.x() + m_matrix(2,1) * point.y() + m_matrix(2,2) * point.z() + m_matrix(2,3);
    const auto w = m_matrix(3,0) * point.x() + m_matrix(3,1) * point.y() + m_matrix(3,2) * point.z() + m_matrix(3,3);

    return {x/w, y/w, z/w};
}

auto
Transform::operator()(const Vec3f& vec) const -> Vec3f {

    const auto x = m_matrix(0,0) * vec.x() + m_matrix(0,1) * vec.y() + m_matrix(0,2) * vec.z();
    const auto y = m_matrix(1,0) * vec.x() + m_matrix(1,1) * vec.y() + m_matrix(1,2) * vec.z();
    const auto z = m_matrix(2,0) * vec.x() + m_matrix(2,1) * vec.y() + m_matrix(2,2) * vec.z();
    return {x, y, z};
}

auto
Transform::operator()(const Normal3f& normal) const -> Normal3f {

    // Note indices: we're using the transpose
    const auto x = m_inverse(0,0) * normal.x() + m_inverse(1,0) * normal.y() + m_inverse(2,0) * normal.z();
    const auto y = m_inverse(0,1) * normal.x() + m_inverse(1,1) * normal.y() + m_inverse(2,1) * normal.z();
    const auto z = m_inverse(0,2) * normal.x() + m_inverse(1,2) * normal.y() + m_inverse(2,2) * normal.z();
    return Normal3f{Vec3f{x, y, z}};
}

auto
Transform::Inverse(const Point3f& point) const -> Point3f {

    const auto x = m_inverse(0,0) * point.x() + m_inverse(0,1) * point.y() + m_inverse(0,2) * point.z() + m_inverse(0,3);
    const auto y = m_inverse(1,0) * point.x() + m_inverse(1,1) * point.y() + m_inverse(1,2) * point.z() + m_inverse(1,3);
    const auto z = m_inverse(2,0) * point.x() + m_inverse(2,1) * point.y() + m_inverse(2,2) * point.z() + m_inverse(2,3);
    const auto w = m_inverse(3,0) * point.x() + m_inverse(3,1) * point.y() + m_inverse(3,2) * point.z() + m_inverse(3,3);

    return {x/w, y/w, z/w};
}



