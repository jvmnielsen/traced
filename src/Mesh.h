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
	Mesh() : Renderable(Vecf(0.18f), Diffuse) {}

	bool Intersects(const Rayf &ray, Intersection &hit_data) override;

	void AddPolygon(std::unique_ptr<Polygon> polygon);

	void TransformByMatrix(const Matrix44f &object_to_world) override;

    void CalculateNormal(Intersection &hit_data) const override
    {
        hit_data.RenderablePtr()->CalculateNormal(hit_data);
    }

	void SetMaterialType(const MaterialType& type) override
	{
    	for (auto& polygon : m_mesh)
		{
    		polygon->SetMaterialType(type);
		}
	}

    void TranslateBy(const Vecf& dir) override
    {
        for (auto& polygon : m_mesh)
            polygon->TranslateBy(dir);
    }

    void RotateAroundX(float dir) override
    {
        for (auto& polygon : m_mesh)
            polygon->RotateAroundX(dir);
    }

    void RotateAroundY(float dir) override
    {
        for (auto& polygon : m_mesh)
            polygon->RotateAroundY(dir);
    }

    void RotateAroundZ(float dir) override
    {
        for (auto& polygon : m_mesh)
            polygon->RotateAroundZ(dir);
    }

    void ScaleBy(float factor) override
    {
        for (auto& polygon : m_mesh)
            polygon->ScaleBy(factor);
    }

private:
    std::vector<std::unique_ptr<Polygon>> m_mesh;
	
};

