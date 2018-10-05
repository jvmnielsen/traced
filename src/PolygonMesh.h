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
	PolygonMesh() : Renderable(Vecf(0.18), Diffuse) {}
	~PolygonMesh() = default;

	bool intersects(const Rayf& ray, HitData& hit_data) override;

	void add_polygon(const std::shared_ptr<Polygon>& polygon);

	void transform_object_to_world(const Matrix44f& object_to_world) override;

    void set_normal(HitData& hit_data) const override
    {
        hit_data.ptr_to_rndrble()->set_normal(hit_data);
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

