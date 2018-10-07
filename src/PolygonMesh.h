#pragma once
#include "Renderable.h"
#include <memory>
#include <vector>
#include "Polygon.h"
#include "Material.h"

class PolygonMesh :
    public Renderable
{
public:
	PolygonMesh() : Renderable(Vecf(0.18f), Diffuse) {}

	bool Intersects(const Rayf &ray, Intersection &hit_data) override;

	void add_polygon(const std::shared_ptr<Polygon>& polygon);

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

private:
    std::vector<std::shared_ptr<Polygon>> m_mesh;
	
};

