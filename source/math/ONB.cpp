#include "onb.hpp"

ONB::ONB(const Normal3f& normal) {

    m_basis[2] = normal;

    Normal3f a;
    if (normal.x > 0.9) { // is the normal the x-axis?
        a = Normal3f{0.0, 1.0, 0.0};
    }
    else {
        a = Normal3f{1.0, 0.0, 0.0};
    }

    m_basis[1] = Cross(m_basis[2], a);
    m_basis[0] = Cross(m_basis[2], m_basis[1]);
}

auto
ONB::ConvertToLocal(const Vec3f& vec) const -> Vec3f {
    return m_basis[0] * vec.x + m_basis[1] * vec.y + m_basis[2] * vec.z;
}

auto
ONB::WorldToLocal(const Normal3f& n) const -> Vec3f {
    return Vec3f(Dot(n, Tangent()), Dot(n, Bitangent()), Dot(n, Normal()));
}

auto
ONB::LocalToWorld(const Normal3f& n) const -> Vec3f {
    return { Tangent().x * n.x + Bitangent().x * n.y + Normal().x * n.z,
             Tangent().y * n.x + Bitangent().y * n.y + Normal().y * n.z,
             Tangent().z * n.x + Bitangent().z * n.y + Normal().z * n.z };
}

auto
ONB::operator[](int i) const -> const Normal3f&
{
    return m_basis[i];
} 