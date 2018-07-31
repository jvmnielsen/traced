#pragma once

struct hit_record;

#include "Scene.h"
#include "Ray.h"
#include "Hitable.h"

Vec3f reflect( const Vec3f& v, const Vec3f& n );

bool refract( const Vec3f& v, const Vec3f& n, float ni_over_nt, Vec3f& refracted );

float schlick( float cosine, float refractive_index );



class Material
{
public:
	virtual bool scatter(const Rayf& ray_in, const hit_record& rec, Vec3f& attenuation, Rayf& scattered) const = 0;
};

class Lambertian :
	public Material
{
public:
	Lambertian(const Vec3f& albedo) : m_albedo(albedo) {}

	virtual bool scatter(const Rayf& ray_in, const hit_record& rec, Vec3f& attenuation, Rayf& scattered) const
	{
		Vec3f target = rec.p + rec.normal + Scene::random_in_unit_sphere();
		scattered = Rayf(rec.p, target - rec.p);
		attenuation = m_albedo;
		return true;
	}

	Vec3f m_albedo;
};



class Metal :
	public Material
{
public:
	Metal(const Vec3f& albedo) : m_albedo(albedo) {}

	virtual bool scatter(const Rayf& ray_in, const hit_record& rec, Vec3f& attenuation, Rayf& scattered) const
	{
		Vec3f reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = Rayf(rec.p, reflected);
		attenuation = m_albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	Vec3f m_albedo;
};

class Dielectric
    : public Material
{
public: 
    Dielectric( float refractive_index ) : m_refractive_index( refractive_index ) {}

    virtual bool scatter( const Rayf& ray_in, const hit_record& rec, Vec3f& attenuation, Rayf& scattered ) const
    {
        // clean-up, especially all the "scattered ="
        Vec3f outward_normal;
        Vec3f reflected = reflect( ray_in.direction(), rec.normal );
        float ni_over_nt;
        attenuation = Vec3f( 1.0, 1.0, 1.0 );
        Vec3f refracted;

        float reflect_prob;
        float cosine;

        if (dot( ray_in.direction(), rec.normal ) > 0)
        {
            outward_normal = -1 * rec.normal; // fix to allow -rec.normal
            ni_over_nt = m_refractive_index;
            cosine = m_refractive_index * dot( ray_in.direction(), rec.normal ) / ray_in.direction().length();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / m_refractive_index;
            cosine = -1 * dot( ray_in.direction(), rec.normal ) / ray_in.direction().length();
        }

        if (refract( ray_in.direction(), outward_normal, ni_over_nt, refracted ))
        {
            reflect_prob = schlick( cosine, m_refractive_index );
        }
        else
        {
            scattered = Rayf( rec.p, reflected );
            reflect_prob = 1.0;
        }

        std::mt19937 generator{ std::random_device()() };
        std::uniform_real_distribution<> distribution( 0, 1 );

        if (distribution( generator ) < reflect_prob)
        {
            scattered = Rayf( rec.p, reflected );
        }
        else
        {
            scattered = Rayf( rec.p, refracted );
        }

        return true;
    }

private:
    float m_refractive_index;
};



