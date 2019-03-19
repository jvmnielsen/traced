#pragma once

#include "matrix4x4.hpp"
#include "vec3.hpp"
#include "point3.hpp"
#include "normal3.hpp"

class Transform
{
public:

    
    //Transform(const Matrix4x4f& matrix) : m_matrix(matrix) { }
    Transform() = default;

    /*
    explicit Transform(const Matrix4x4f& mat) : m_matrix(mat) {
        m_inverse = m_matrix.Inverse();
    }*/

    //Matrix4x4f GetMatrix() const { return m_matrix; }
    //Matrix4x4f GetInverseMatrix() const { return m_inverse; }

    auto Translate(const Vec3f& vec) -> Transform&;
    auto Scale(const Vec3f& vec) -> Transform&;
    auto Rotate(const Vec3f& axis, float angle) -> Transform&;

    auto operator()(const Point3f& point) const -> Point3f;
    auto operator()(const Vec3f& vec) const -> Vec3f;
    auto operator()(const Normal3f& normal) const -> Normal3f;

    auto Inverse(const Point3f& p) const -> Point3f;
    auto Inverse(const Vec3f& p) const -> Vec3f;
    auto Inverse(const Normal3f& p) const -> Normal3f;
    //auto Inverse() const -> Transform;

    Matrix4x4f m_matrix;
    Matrix4x4f m_inverse;
};


/*
auto 
Translate(const Vec3f& vec) -> Transform {


    const Matrix4x4f mat = {
            1, 0, 0, vec.x,
            0, 1, 0, vec.y,
            0, 0, 1, vec.z,
            0, 0, 0, 1
    };
    const Matrix4x4f invMat = {
            1, 0, 0, -vec.x,
            0, 1, 0, -vec.y,
            0, 0, 1, -vec.z,
            0, 0, 0, 1
    };

    return {mat, invMat};
}

auto
Scale(const Vec3f& vec) -> Transform {
    const Matrix4x4f mat = {
        vec.x,     0,     0, 0,
            0, vec.y,     0, 0,
            0,     0, vec.z, 0,
            0,     0,     0, 1
    };

    const Matrix4x4f invMat = {
        1.0f / vec.x,          0,         0,  0,
        0,          1.0f / vec.y,         0,  0,
        0,                  0, 1.0f / vec.z,  0,
        0,                  0,         0,  1
    };

    return {mat, invMat};
}

auto
Rotate(const Vec3f& axis, float angle) -> Transform {

    const auto normAxis = Normalize(axis);
    const auto rad = Math::DegreeToRadian(angle);

    const auto cosTheta = std::cos(rad);
    const auto sinTheta = std::sin(rad);

    Matrix4x4f mat;

    mat(0, 0) = normAxis.x * normAxis.x + (1.0f - normAxis.x * normAxis.x) * cosTheta;
    mat(0, 1) = normAxis.x * normAxis.y * (1.0f - cosTheta) - normAxis.z * sinTheta;
    mat(0, 2) = normAxis.x * normAxis.z * (1.0f - cosTheta) + normAxis.y * sinTheta;
    mat(0, 3) = 0;

    mat(1, 0) = normAxis.x * normAxis.y * (1.0f - cosTheta) + normAxis.z * sinTheta;
    mat(1, 1) = normAxis.y * normAxis.y + (1.0f - normAxis.y * normAxis.y) * cosTheta;
    mat(1, 2) = normAxis.y * normAxis.z * (1.0f - cosTheta) - normAxis.x * sinTheta;
    mat(1, 3) = 0;

    mat(2, 0) = normAxis.x * normAxis.z * (1.0f - cosTheta) - normAxis.y * sinTheta;
    mat(2, 1) = normAxis.y * normAxis.z * (1.0f - cosTheta) + normAxis.x * sinTheta;
    mat(2, 2) = normAxis.z * normAxis.z + (1.0f - normAxis.z * normAxis.z) * cosTheta;
    mat(2, 3) = 0;

    mat(3, 0) = 0;
    mat(3, 1) = 0;
    mat(3, 2) = 0;
    mat(3, 3) = 1;

    return {mat, mat.Transpose()};
} */
