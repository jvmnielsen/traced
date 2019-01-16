#pragma once

#include <optional>
#include "../math/point3.hpp"
#include "../math/normal3.hpp"
#include "../math/point2.hpp"
#include "../imaging/color3.hpp"
#include "../math/ray.hpp"
#include "sampler.hpp"
#include "../material/material.hpp"

class Mesh;

struct SamplingInfo {
    Vec3f toLight;
    Vec3f toEye;
    float pdf;
};


class Intersection
{
public:
    Intersection() {}

    Intersection(Point3f point, Point2f uvCoord, Normal3f geometricNormal, Normal3f shadingNormal);

    //auto Emitted() const -> Color3f;
    auto NewThroughput(const Color3f& currentThroughput, SamplingInfo& info, Sampler& sampler) -> Color3f;
    auto UpdateRayToSampleDir(Rayf& rayToUpdate, const SamplingInfo& info) const -> void;

    Point3f OffsetShadingPoint() const;
    Point3f OffsetGeometricPoint() const;

    auto GetTangent() const -> const Normal3f&;
    auto GetPoint() const -> const Point3f&;
    auto GetGeometricNormal() const -> const Normal3f&;
    auto GetShadingNormal() const -> const Normal3f&;
    auto IsSpecular() const -> bool;

    Normal3f    m_tangent;

    const Mesh*           m_mesh;

    const Material*       m_material;

    std::optional<int> m_lightID;

private:


    Point3f         m_point;
    Point2f         m_uv;
    //Triangle*       m_triangle;
    Normal3f        m_geometricNormal;
    Normal3f        m_shadingNormal;
};