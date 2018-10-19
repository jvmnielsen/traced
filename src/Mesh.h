#pragma once
#include "Shape.h"
#include <memory>
#include <vector>
#include "Triangle.h"
#include "Material.h"

class Mesh :
    public Shape
{
public:
	Mesh() : Shape(Color3f(0.18f), nullptr) {}

	bool Intersects(const Rayf &ray, Intersection &hit_data) override;
    bool IntersectsQuick(const Rayf& ray) const override;


	void AddPolygon(std::shared_ptr<Triangle> polygon);

    void CalculateNormal(Intersection &hit_data) const override;

	std::shared_ptr<BoundingVolume> GetBoundingVolume() const override;

	//void SetMaterialType(const MaterialType& type) override;

    void TransformBy(const Transform& transform) override;

    //std::shared_ptr<Mesh> CloneMesh(); // consider making ctor

private:
    std::vector<std::shared_ptr<Triangle>> m_triangles;
};

