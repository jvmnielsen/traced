#pragma once
#include <array>
#include "../math/point2.hpp"
#include "normal3.hpp"

class ONB {
public:

    explicit ONB(const Normal3f& normal);
    
    auto operator[](int i) const -> const Normal3f&;
    auto convert_to_local(const Vec3f& vec) const -> Vec3f;
    //auto ConvertFromLocal()


    //auto WorldToLocal(const Normal3f& n) const -> Vec3f;
    //auto LocalToWorld(const Normal3f& n) const -> Vec3f;

    auto w() const -> const Normal3f& { return m_basis[2]; }
    auto v() const -> const Normal3f& { return m_basis[1]; }
    auto u() const -> const Normal3f& { return m_basis[0]; }
    

private:
    std::array<Normal3f, 3> m_basis;
};

