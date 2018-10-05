#pragma once
#include "Renderable.h"

class Sphere :
    public Renderable
{
public:
    Sphere(const Vecf& center, const float radius, const Vecf& albedo, MaterialType material)
        : Renderable(albedo, material)
		, m_center(center) 
        , m_radius(radius)
        , m_radius_squared(radius * radius)
    {}

    bool intersects(const Rayf& ray, HitData& hit_data) override;

    bool solve_quadratic( const float a, const float b, const float c, float& solu_one, float& solu_two ) const;

    void transform_object_to_world(const Matrix44f& object_to_world) override { }

    void set_normal(HitData& hit_data) const override;
	

private:
    Vecf m_center;
    float m_radius;
    float m_radius_squared;
	//Material* m_material;
    
};

