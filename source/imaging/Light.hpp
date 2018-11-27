#pragma once
#include "../math/vec3.hpp"
#include "color3.hpp"
#include "../math/point3.hpp"
#include "../math/math_util.hpp"
#include "../geometry/mesh.hpp"


struct LightingAtPoint
{
    Vec3f directionToLight;
    Color3f intensityAtPoint;
};

class Light
{
public:
    Light(std::shared_ptr<Emissive> material, std::unique_ptr<Mesh> mesh);

    auto Sample(const Intersection& ref, Normal3f& wi, float& pdf, Intersection& atLight) const -> Color3f;

protected:

    std::shared_ptr<Emissive>   m_material;
    std::unique_ptr<Mesh>       m_mesh;
};

