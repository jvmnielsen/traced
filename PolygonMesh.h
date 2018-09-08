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

	bool intersects(const Rayf& ray, float& t, Vec3f& intercpt_coord) override;

	void add_polygon(const std::shared_ptr<Polygon>& polygon);

	void transform_object_to_world(const Matrix44f& object_to_world) override;

private:
    std::vector<std::shared_ptr<Polygon>> m_mesh;
	
};

