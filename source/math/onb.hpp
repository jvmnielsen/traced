#pragma once
#include "../math/normal3.hpp"
#include <array>
#include "../math/point2.hpp"

class ONB {
public:
  
    ONB(const Normal3f& normal);
    
    auto operator[](int i) const -> const Normal3f&;
    auto ConvertToLocal(const Vec3f& vec) const -> Vec3f;
    //auto ConvertFromLocal()


    //auto WorldToLocal(const Normal3f& n) const -> Vec3f;
    //auto LocalToWorld(const Normal3f& n) const -> Vec3f;

    auto W() const -> const Normal3f& { return m_basis[2]; }
    auto V() const -> const Normal3f& { return m_basis[1]; }
    auto U() const -> const Normal3f& { return m_basis[0]; }
    

private:
    std::array<Normal3f, 3> m_basis;
};

