#pragma once
#include "Ray.h"
#include "Matrix44.h"
#include "HitData.h"

class Material;

class Renderable
{
public:
	explicit Renderable(const float albedo) : m_albedo(albedo) {}

	//Renderable(const Vec3f& albedo) : m_albedo(albedo) {}
    //virtual ~Renderable() = default;
    //virtual ~Renderable() = default;
    virtual bool intersects(const Rayf& ray, HitData& hit_data) = 0;

	virtual void transform_object_to_world(const Matrix44f& object_to_world) = 0;

    virtual Vec3f get_surface_properties(HitData& hit_data) const = 0;

	float m_albedo;

private:
	//Matrix44f m_object_to_world;
};

class Plane
    : public Renderable
{
public:
    Plane(const Vec3f& normal, const Vec3f& point, const float albedo)
        : Renderable(albedo)
		, m_normal(normal)
        , m_point_on_plane( point )
    {}

    bool intersects(const Rayf& ray, HitData& hit_data) override
    {
        const auto denom = dot( m_normal, ray.direction() );
        
        if ( denom > 1e-6 )
        {
            const auto center_to_intercept = m_point_on_plane - ray.origin();

            hit_data.m_t = dot( center_to_intercept, m_normal ) / denom;

            return ( hit_data.m_t >= 0 );
        }

        return false;
    }

	void transform_object_to_world(const Matrix44f& object_to_world) override
    {
	    
    }

	Vec3f get_surface_properties(HitData& hit_data) const override
    {
		return { 255,255,255 };
    }

private:
    Vec3f m_normal;
    Vec3f m_point_on_plane;
	
};


class Disk
    : public Renderable
{
public:
    Disk(const Vec3f& normal, const Vec3f& point, const float radius, const float albedo)
        : Renderable(albedo)
		, m_normal(normal)
        , m_point_on_plane(point)
        , m_radius(radius)
        , m_radius_squared(radius * radius)
    {}

    bool intersects(const Rayf& ray, HitData& hit_data) override
    {

        // first part is, unsurprisingly, the same as for a disk
        // consider inheriting from disk

        const auto denom = dot( m_normal, ray.direction() );

        if ( !( denom > 1e-6 ) )
            return false;

        const auto recenter = m_point_on_plane - ray.origin();

        hit_data.m_t = dot( recenter, m_normal ) / denom;

        if ( hit_data.m_t < 0 )
            return false;

        const auto supposed_intercept = ray.point_at_parameter( hit_data.m_t );
        const auto center_to_intercept = supposed_intercept - m_point_on_plane;
        const auto difference = dot( center_to_intercept, center_to_intercept );

        return ( difference <= m_radius_squared );
    }

	void transform_object_to_world(const Matrix44f& object_to_world) override
	{

	}

	Vec3f get_surface_properties(HitData& hit_data) const override
	{
		return { 255,255,255 };
	}

private:
    Vec3f m_normal;
    Vec3f m_point_on_plane;
    float m_radius;
    float m_radius_squared;
};


class AABB 
    : public Renderable
{
   
    AABB(const Vec3f& low_bound, const Vec3f& high_bound, const float albedo)
        : Renderable(albedo)
    {
        m_bounds[0] = low_bound;
        m_bounds[1] = high_bound;
    }
    
    bool intersects(const Rayf& ray, HitData& hit_data) override
    {
        float t_min, t_max, ty_min, ty_max, tz_min, tz_max;

        t_min = (m_bounds[ray.m_sign[0]].x() - ray.origin().x()) * ray.m_inv_dir.x();
        t_max = (m_bounds[1 - ray.m_sign[0]].x() - ray.origin().x()) * ray.m_inv_dir.x();
        ty_min = (m_bounds[ray.m_sign[1]].y() - ray.origin().y()) * ray.m_inv_dir.y();
        ty_max = (m_bounds[1 - ray.m_sign[1]].y() - ray.origin().y()) * ray.m_inv_dir.y();


        if (t_min > ty_max || ty_min > t_max)
            return false;
        if (ty_min > t_min)
            t_min = ty_min;
        if (ty_max < t_max)
            t_max = ty_max;

        tz_min = (m_bounds[ray.m_sign[2]].z() - ray.origin().z()) * ray.m_inv_dir.z();
        tz_max = (m_bounds[1 - ray.m_sign[2]].z() - ray.origin().z()) * ray.m_inv_dir.z();

        if (t_min > tz_max || tz_min > t_max)
            return false;
        if (tz_min > t_min)
            t_min = tz_min;
        if (tz_max < t_max)
            t_max = tz_max;

        return true;
        
    }

    void transform_object_to_world(const Matrix44f& object_to_world) override
    {

    }

    Vec3f get_surface_properties(HitData& hit_data) const override
    {
        return {255,255,255};
    }

private:

    Vec3f m_bounds[2];
};
 