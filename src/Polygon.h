#pragma once
#include "Renderable.h"
#include "MathUtil.h"

class Polygon :
    public Renderable
{
public:
    Polygon() = default;

    Polygon(
            const Vec3f& vertx0,
            const Vec3f& vertx1,
            const Vec3f& vertx2,
            bool is_single_sided,
            const Vec3f& albedo,
            MaterialType material)
        : Renderable(albedo, material)
        , m_vertices{vertx0, vertx1, vertx2}
        , m_edge0(vertx1 - vertx0)
        , m_edge0_2(vertx2 - vertx0)
        , m_edge1(vertx2 - vertx1)
        , m_edge2(vertx0 - vertx2)
        , m_is_single_sided(is_single_sided)
        , m_normal(((vertx1 - vertx0).CrossProduct(vertx2 - vertx0)).Normalize())
    {
    }

    Polygon(
        const Vec3f& vertx0,
        const Vec3f& vertx1,
        const Vec3f& vertx2,
        const Vec3f& vertx_normal0,
        const Vec3f& vertx_normal1,
        const Vec3f& vertx_normal2,
        bool is_single_sided,
        const Vec3f& albedo,
        MaterialType material)
        : Renderable(albedo, material)
        , m_vertices{vertx0, vertx1, vertx2}
        , m_vertex_normals{vertx_normal0, vertx_normal1, vertx_normal2}
        , m_edge0(vertx1 - vertx0)
        , m_edge0_2(vertx2 - vertx0)
        , m_edge1(vertx2 - vertx1)
        , m_edge2(vertx0 - vertx2)
        , m_is_single_sided(is_single_sided)
        , m_normal((vertx1 - vertx0).CrossProduct(vertx2 - vertx0).Normalize())
    {
    }

    //Polygon(const Polygon& other) = default;

    bool Intersects(const Rayf &ray, Intersection &hit_data) override;

    void TransformByMatrix(const Matrix4x4f &object_to_world) override;
    void TransformByMatrix2(const Matrix4x4f &object_to_world);

    void SetMaterialType(const MaterialType& type) override
    {
        m_material = type;
    }

    void CalculateNormal(Intersection &hit_data) const override;

    void TranslateBy(const Vec3f& dir) override {}

    void RotateAroundX(float angle) override {}
    void RotateAroundY(float angle) override {}
    void RotateAroundZ(float angle) override {}

    void ScaleBy(float factor) override {}


    Vec3f m_vertices[3];  // TODO: encapsulate

    Vec3f m_vertex_normals[3];

private:

	void UpdateEdges();

    Vec3f m_edge0;
    Vec3f m_edge0_2; // from vertx 0 to vertx 2. used for normal
    Vec3f m_edge1;
    Vec3f m_edge2;
    Vec3f m_normal;
    bool m_is_single_sided;
    float m_epsilon = 1e-8f;
};

