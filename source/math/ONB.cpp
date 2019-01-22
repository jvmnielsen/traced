#include "ONB.h"



ONB::ONB(const Normal3f& axis) {

    m_basis[2] = axis;

    Vec3f a;
    if (axis.x > 0.9f) {
        a = Vec3f{0, 1, 0};
    }
    else {
        a = Vec3f{1, 0, 0};
    }

    m_basis[1] = Cross(m_basis[2], a).Normalize();
    m_basis[0] = Cross(m_basis[2], m_basis[1]);
}

auto
ONB::ConvertToLocal(const Vec3f& vec) const -> Vec3f{
    return vec.x * u() + vec.y * v() + vec.z * w();
}

auto
ONB::operator[](int i) const -> const Normal3f&
{
    return m_basis[i];
}