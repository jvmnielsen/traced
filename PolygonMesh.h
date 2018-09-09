#pragma once
#include "RenderPrimitive.h"
#include <memory>
#include <vector>
#include "Polygon.h"
#include "Material.h"

class PolygonMesh :
    public RenderPrimitive
{
public:
	PolygonMesh() = default;
	~PolygonMesh() = default;

	bool intersects(const Rayf& ray, HitData& hit_data) override;

	void add_polygon(const std::shared_ptr<Polygon>& polygon);

	void transform_object_to_world(const Matrix44f& object_to_world) override;

	Vec3f get_surface_properties(HitData& hit_data) const override;


private:
    std::vector<std::shared_ptr<Polygon>> m_mesh;
	
};

