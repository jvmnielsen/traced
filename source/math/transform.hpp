#pragma once

#include "matrix4x4.hpp"
#include "vec3.hpp"
#include "point3.hpp"
#include "normal3.hpp"

class Transform
{
public:
    Transform()
    {
        m_mat.setIdentity();
        m_invMat.setIdentity();
    }

    Transform(const Matrix4x4f& mat, const Matrix4x4f& invMat) : m_mat(mat), m_invMat(invMat) { }

    explicit Transform(const Matrix4x4f& mat) : m_mat(mat)
    {
        m_invMat = m_mat.Invert();
    }

    Matrix4x4f GetMatrix() const { return m_mat; }
    Matrix4x4f GetInverseMatrix() const { return m_invMat; }

    static Transform Translate(const Vec3f& vec);
    static Transform Scale(const Vec3f& vec);
    static Transform Rotate(const Vec3f& axis, float angle);
    // static Transform Perspective(float FieldOfView, float ClipNear, float ClipFar); // not needed for now

    //auto TransformAffine(const Point3f& point) const -> Point3f;
    void operator()(Point3f& point) const;
    void operator()(Vec3f& vec) const;
    void operator()(Normal3f& normal) const;


private:
    Matrix4x4f m_mat;
    Matrix4x4f m_invMat;
};