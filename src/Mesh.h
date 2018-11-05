#pragma once
#include <memory>
#include <vector>
#include "Triangle.h"
#include <random>

class Mesh :
    public Shape
{
public:
	Mesh() 
        : Shape(nullptr) 
        , m_gen(std::random_device()())
        , m_realDistribution(0.0f, 1.0f)
    {}

	bool Intersects(const Rayf &ray, Intersection &hit_data) override;
    bool IntersectsQuick(const Rayf& ray) const override;


	void AddPolygon(std::unique_ptr<Triangle> polygon);

    //void NormalAtIntesection(const Intersection &hit_data) const override;

    std::unique_ptr<BoundingVolume> GetBoundingVolume() const override;

    Point3f GetPointOnSurface(const float u, const float v) const override;
    Point3f GetRandomPointOnSurface() override;
    Intersection GetRandomSurfaceIntersection() override;

    Normal3f CalculateShadingNormal(const Intersection& isect) const override;

    void TransformBy(const Transform& transform) override;

    std::unique_ptr<Mesh> Clone(); // consider making ctor

    //auto SetParentMeshMaterial(std::shared_ptr<Material> material) -> void override;


private:
    std::vector<std::unique_ptr<Triangle>> m_triangles;

    int GetRandomTriangleVertex();

    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_indexDist;
    std::uniform_real_distribution<float> m_realDistribution;

};

