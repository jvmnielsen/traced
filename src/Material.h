#pragma once

#include "Scene.h"
#include "Ray.h"
#include "Renderable.h"

//Vecf Reflect(const Vecf& v, const Vecf& n);

bool refract(const Vecf& v, const Vecf& n, float ni_over_nt, Vecf& refracted);

float schlick(float cosine, float refractive_index);

class Material
{
public:
	virtual Rayf scatter(const Rayf& ray, const HitData& hit_data) const = 0;
};



class Lambertian :
	public Material
{
public:
	Lambertian(const Vecf& albedo) 
        : m_albedo(albedo) 
        , m_gen( std::random_device()() )
        , m_dist( 0, 1 ) {}

    Vecf random_in_unit_sphere();

    Rayf scatter(const Rayf& ray, const HitData& hit_data) const override;
	

	Vecf m_albedo;
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dist;

};


/*
class Metal :
	public Material
{
public:
	Metal(const Vecf& albedo) : m_albedo(albedo) {}

	virtual bool color_point(const Rayf& ray_in, const hit_data& rec, Vecf& attenuation, Rayf& scattered)
	{
		Vecf reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = Rayf(rec.p, reflected);
		attenuation = m_albedo;
		return (DotProduct(scattered.direction(), rec.normal) > 0);
	}

	Vecf m_albedo;
};

class Dielectric
    : public Material
{
public: 
    Dielectric( float refractive_index ) : m_refractive_index( refractive_index ) {}

    virtual bool color_point( const Rayf& ray_in, const hit_data& rec, Vecf& attenuation, Rayf& scattered )
    {
        // clean-up, especially all the "scattered ="
        Vecf outward_normal;
        Vecf reflected = Reflect( ray_in.direction(), rec.normal );
        float ni_over_nt;
        attenuation = Vecf( 1.0, 1.0, 1.0 );
        Vecf refracted;

        float reflect_prob;
        float cosine;

        if (DotProduct( ray_in.direction(), rec.normal ) > 0)
        {
            outward_normal = -1 * rec.normal; // fix to allow -rec.normal
            ni_over_nt = m_refractive_index;
            cosine = m_refractive_index * DotProduct( ray_in.direction(), rec.normal ) / ray_in.direction().Magnitude();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / m_refractive_index;
            cosine = -1 * DotProduct( ray_in.direction(), rec.normal ) / ray_in.direction().Magnitude();
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
}; */ 



