#pragma once
#include "Shape.h"
#include "MathUtil.h"

class Triangle :
    public Shape
{
public:
    Triangle() : Shape(nullptr) { }

    Triangle(std::array<Point3f, 3> vertices,
             std::array<Normal3f, 3> vertexNormals,
             bool isSingleSided,
             std::shared_ptr<Material> material);

    bool Intersects(const Rayf &ray, Intersection &hit_data) override;
    bool IntersectsQuick(const Rayf& ray) const override;


    //void SetMaterialType(const MaterialType& type) override { m_material = type; }

    Normal3f CalculateShadingNormal(const Intersection& intersec) const override;

    void TransformBy(const Transform& transform) override;
   
    Point3f GetPointOnSurface(const float u, const float v) const override;
    Point3f GetRandomPointOnSurface() override;
    Intersection GetRandomSurfaceIntersection() override;

    const std::array<Point3f, 3>& GetVertices() const;
    std::unique_ptr<BoundingVolume> GetBoundingVolume() const override;


private:

    std::array<Point3f, 3> m_vertices;
    //Point3f test;
    //Point3f m_vertex[3];  // TODO: encapsulate
    std::array<Normal3f, 3> m_vertexNormals;
    //Normal3f m_normals[3];
    std::array<Vec3f, 2> m_edges; // Pre-calculated for use in Intercept function

	void UpdateEdges();

    /*
    Vec3f m_edge0;
    Vec3f m_edge0_2; // from vertx 0 to vertx 2. used for normal
    Vec3f m_edge1;
    Vec3f m_edge2;
    */

    Vec3f m_faceNormal;
    bool m_isSingleSided;
    float m_epsilon = 1e-8f;
};

