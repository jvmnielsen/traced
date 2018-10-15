#pragma once
#include "Renderable.h"
#include <memory>
#include <vector>
#include "Polygon.h"
#include "Material.h"

class Mesh :
    public Renderable
{
public:
	Mesh() : Renderable(Vec3f(0.18f), Diffuse) {}

	bool Intersects(const Rayf &ray, Intersection &hit_data) override;

	void AddPolygon(std::unique_ptr<Polygon> polygon);

	void TransformByMatrix(const Matrix4x4f &object_to_world) override;

    void CalculateNormal(Intersection &hit_data) const override;

	void SetMaterialType(const MaterialType& type) override;

    void TranslateBy(const Vec3f& dir) override;

    void RotateAroundX(float angle) override;

    void RotateAroundY(float angle) override;

    void RotateAroundZ(float angle) override;

    void ScaleBy(float factor) override;

    std::unique_ptr<Mesh> CloneMesh(); // consider making ctor
private:
    std::vector<std::unique_ptr<Polygon>> m_mesh;
	
};

