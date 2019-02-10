#pragma once
#include "../math/normal3.hpp"
#include <array>

class ONB {
public:
  
    ONB(const Normal3f&  axis);
    auto operator[](int i) const -> const Normal3f&;
    auto ConvertToLocal(const Vec3f& vec) const -> Vec3f;
    auto u() const -> Normal3f { return m_basis[0]; }
    auto v() const -> Normal3f { return m_basis[1]; }
    auto w() const -> Normal3f { return m_basis[2]; }

private:
    std::array<Normal3f, 3> m_basis;
};

