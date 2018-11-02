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
	Mesh() : Shape(nullptr) {}

	bool Intersects(const Rayf &ray, Intersection &hit_data) override;
    bool IntersectsQuick(const Rayf& ray) const override;


	void AddPolygon(std::unique_ptr<Triangle> polygon);

    //void NormalAtIntesection(const Intersection &hit_data) const override;

    std::unique_ptr<BoundingVolume> GetBoundingVolume() const override;

	//void SetMaterialType(const MaterialType& type) override;

    void TransformBy(const Transform& transform) override;

    std::shared_ptr<Mesh> Clone(); // consider making ctor

private:
    std::vector<std::unique_ptr<Triangle>> m_triangles;
};

