#pragma once
#include "Shape.h"
#include "MathUtil.h"
#include "BoundingVolume.h"
#include "Intersection.h"

class Triangle :
    public Shape
{
public:
    Triangle() : Shape(nullptr) { }

    Triangle(
            const Point3f& vertx0,
            const Point3f& vertx1,
            const Point3f& vertx2,
            bool is_single_sided,
  
            std::shared_ptr<Material> material)
        : Shape(material)
        , m_vertex{vertx0, vertx1, vertx2}
        , m_edge0(vertx1 - vertx0)
        , m_edge0_2(vertx2 - vertx0)
        , m_edge1(vertx2 - vertx1)
        , m_edge2(vertx0 - vertx2)
        , m_is_single_sided(is_single_sided)
        , m_normal(((vertx1 - vertx0).CrossProduct(vertx2 - vertx0)).Normalize())
    {
    }

    Triangle(
        const Point3f& vertx0,
        const Point3f& vertx1,
        const Point3f& vertx2,
        const Vec3f& vertx_normal0,
        const Vec3f& vertx_normal1,
        const Vec3f& vertx_normal2,
        bool is_single_sided,
       
        std::shared_ptr<Material> material)
        : Shape(material)
        , m_vertex{vertx0, vertx1, vertx2}
        , m_normals{vertx_normal0, vertx_normal1, vertx_normal2}
        , m_edge0(vertx1 - vertx0)
        , m_edge0_2(vertx2 - vertx0)
        , m_edge1(vertx2 - vertx1)
        , m_edge2(vertx0 - vertx2)
        , m_is_single_sided(is_single_sided)
        , m_normal((vertx1 - vertx0).CrossProduct(vertx2 - vertx0).Normalize())
    {
    }

    bool Intersects(const Rayf &ray, Intersection &hit_data) override;
    bool IntersectsQuick(const Rayf& ray) const override;


    //void SetMaterialType(const MaterialType& type) override { m_material = type; }

    std::shared_ptr<BoundingVolume> GetBoundingVolume() const override { return nullptr; }

    void CalculateNormal(Intersection &intersec) const override;

    void TransformBy(const Transform& transform) override;

    Point3f m_vertex[3];  // TODO: encapsulate

    Normal3f m_normals[3];

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

