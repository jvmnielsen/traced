#pragma once

#include <optional>
#include "../math/point3.hpp"
#include "../math/point2.hpp"
#include "../material/material.hpp"
#include "../math/ray.hpp"

class Mesh;

class Intersection
{
public:

    Intersection(Point3f point, Point2f uv_coord, Vec3f geometric_normal, Vec3f shading_normal);


    Point3f offset_point() const;

   
    auto point() const -> const Point3f&;
    auto geometric_normal() const -> const Vec3f&;
    auto shading_normal() const -> const Vec3f&;

    auto IsSpecular() const -> bool;

    auto GetShadingBasis() const -> const ONB& { return m_shading_basis; }
    //auto GetTransformedSampledVec(Sampler& sampler) const -> Normal3f;



    auto sample_material(const Vec3f& wo, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f>;
    auto emitted(const Vec3f& dir) const -> Color3f;
    auto material_pdf(const Vec3f& wi) const->FLOAT;
    auto evaluate_material(const Vec3f& wo, const Vec3f& wi) const->Color3f;

    auto ray_towards(const Vec3f& dir) const-> Rayf;


    auto SetMeshAndMaterial(const Mesh* mesh, const Material* material) -> void;

    std::optional<int> m_lightID;

private:

    const Mesh*           m_mesh;

    const Material*       m_material;

    


    ONB m_shading_basis;

    Point3f         m_point;
    Point2f         m_uv;
    Vec3f           m_geometric_normal;
    //Normal3f        m_shadingNormal;
    //Normal3f        m_tangent;
};