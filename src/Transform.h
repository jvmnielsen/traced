#ifndef RAYTRACER_TRANSFORMATION_H
#define RAYTRACER_TRANSFORMATION_H

#include "MathUtil.h"


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

    //auto TransformAffine(const Point3f& point) const -> Point3f;
    Point3f operator()(const Point3f& point) const;
    Vec3f operator()(const Vec3f& vec) const;
    Normal3f operator()(const Normal3f& normal) const;

private:
    Matrix4x4f m_mat;
    Matrix4x4f m_invMat;
};



#endif //RAYTRACER_TRANSFORMATION_H
