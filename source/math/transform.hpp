#pragma once

#include "matrix4x4.hpp"
#include "vec3.hpp"
#include "point3.hpp"
#include "normal3.hpp"
//#include "../math/math_util.hpp"

class Transform
{
public:

    
    Transform(const Matrix4x4f& mat, const Matrix4x4f& invMat) : m_mat(mat), m_invMat(invMat) {}
    Transform() : m_mat(Matrix4x4f::getIdentity()), m_invMat(Matrix4x4f::getIdentity()) { }

    /*
    explicit Transform(const Matrix4x4f& mat) : m_mat(mat) {
        m_invMat = m_mat.Invert();
    }*/

    //Matrix4x4f GetMatrix() const { return m_mat; }
    //Matrix4x4f GetInverseMatrix() const { return m_invMat; }

    auto Translate(const Vec3f& vec) -> Transform&;
    auto Scale(const Vec3f& vec) -> Transform&;
    auto Rotate(const Vec3f& axis, float angle) -> Transform&;
    // static Transform Perspective(float FieldOfView, float ClipNear, float ClipFar); // not needed for now

    //auto TransformAffine(const Point3f& point) const -> Point3f;
    void operator()(Point3f& point) const;
    void operator()(Vec3f& vec) const;
    void operator()(Normal3f& normal) const;
private:

    

    Matrix4x4f m_mat;
    Matrix4x4f m_invMat;
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
