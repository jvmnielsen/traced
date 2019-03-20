#include "onb.hpp"

ONB::ONB(const Vec3f& normal) {

    m_basis[2] = normal;

    Vec3f a;
    if (std::abs(normal.x()) > 0.9) { // is the normal the x-axis?
        a = Vec3f{0.0, 1.0, 0.0};
    } else {
        a = Vec3f{1.0, 0.0, 0.0};
    }

    m_basis[1] = cross(m_basis[2], a);
    m_basis[0] = cross(m_basis[1], m_basis[2]);
}

auto
ONB::convert_to_local(const Vec3f& vec) const -> Vec3f {
    return m_basis[0] * vec.x() + m_basis[1] * vec.y() + m_basis[2] * vec.z();
}

/*
auto
ONB::WorldToLocal(const Normal3f& n) const -> Vec3f {
    return Vec3f(Dot(n, U()), Dot(n, V()), Dot(n, W()));
}

auto
ONB::LocalToWorld(const Normal3f& n) const -> Vec3f {
    return { U().x * n.x + V().x * n.y + W().x * n.z,
             U().y * n.x + V().y * n.y + W().y * n.z,
             U().z * n.x + V().z * n.y + W().z * n.z };
} */

auto
ONB::operator[](int i) const -> const Vec3f&
{
    return m_basis[i];
} 