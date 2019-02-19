#include "onb.hpp"



ONB::ONB(const Normal3f& axis) {

    m_basis[2] = axis;

    Normal3f a;
    if (axis.x > 0.9f) {
        a = Normal3f{0, 1, 0};
    }
    else {
        a = Normal3f{1, 0, 0};
    }

    m_basis[1] = Cross(m_basis[2], a);
    m_basis[0] = Cross(m_basis[2], m_basis[1]);
}

auto
ONB::ConvertToLocal(const Vec3f& vec) const -> Vec3f {
    return u() * vec.x + v() * vec.y + w() * vec.z;
}

auto
ONB::operator[](int i) const -> const Normal3f&
{
    return m_basis[i];
} 