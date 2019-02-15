#pragma once

#include <optional>
#include "../math/point3.hpp"
#include "../math/point2.hpp"
#include "../material/material.hpp"

class Mesh;

class Intersection
{
public:

    Intersection(Point3f point, Point2f uvCoord, Normal3f geometricNormal, Normal3f shadingNormal);

    Point3f OffsetShadingPoint() const;
    Point3f OffsetGeometricPoint() const;

   
    auto GetPoint() const -> const Point3f&;
    auto GetGeometricNormal() const -> const Normal3f&;
    auto GetShadingNormal() const -> const Normal3f&;
    auto IsSpecular() const -> bool;

    auto GetOrthonormalBasis() const -> const ONB&;
    auto GetTransformedSampledVec(Sampler& sampler) const -> Vec3f;



    const Mesh*           m_mesh;

    const Material*       m_material;

    std::optional<int> m_lightID;

private:

    ONB m_orthonormal;

    Point3f         m_point;
    Point2f         m_uv;
    Normal3f        m_geometricNormal;
    Normal3f        m_shadingNormal;
};