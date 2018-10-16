#pragma once
#include "Renderable.h"
#include <memory>
#include <vector>
#include "Triangle.h"
#include "Material.h"

class Mesh :
    public Renderable
{
public:
	Mesh() : Renderable(Color3f(0.18f), MaterialType::Diffuse) {}

	bool Intersects(const Rayf &ray, Intersection &hit_data) override;

	void AddPolygon(std::unique_ptr<Triangle> polygon);

    void CalculateNormal(Intersection &hit_data) const override;

	void SetMaterialType(const MaterialType& type) override;

    void TransformBy(const Transform& transform) override;

    std::unique_ptr<Mesh> CloneMesh(); // consider making ctor
private:
    std::vector<std::unique_ptr<Triangle>> m_mesh;
	
};

