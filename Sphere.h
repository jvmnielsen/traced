#pragma once
#include "Renderable.h"

class Sphere :
    public Renderable
{
public:
    Sphere(Vec3f& center, const float radius, const float albedo) 
        : Renderable(albedo)
		, m_center(center) 
        , m_radius(radius)
        , m_radius_squared(radius * radius)
    {}

    bool intersects(const Rayf& ray, HitData& hit_data) override;

    bool solve_quadratic( const float a, const float b, const float c, float& solu_one, float& solu_two ) const;

	void transform_object_to_world(const Matrix44f& object_to_world) override
	{

	}

	Vec3f get_surface_properties(HitData& hit_data) const override
	{
		auto normal = hit_data.m_coord_closest - m_center;
		normal.normalize();
		return normal;
	}

private:
    Vec3f m_center;
    float m_radius;
    float m_radius_squared;
	//Material* m_material;
    
};

