#include "Material.h"
#include "MathUtil.h"

bool refract( const Vec3f& v, const Vec3f& n, float ni_over_nt, Vec3f& refracted )
{
    Vec3f uv = Normalize(v);
    float dt = uv.DotProduct(n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted =  (uv - n * dt) - n * sqrt( discriminant ) * ni_over_nt ;
        return true;
    }
    else
        return false;
}

float schlick( float cosine, float refractive_index )
{
    float r0 = (1 - refractive_index) / (1 + refractive_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow( (1 - cosine), 5 );
}


bool Lambertian::Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) const
{
    const auto target = isect.m_point + isect.m_normal;// + random_in_unit_sphere();
    scattered = Rayf{ isect.m_point, target-isect.m_point };
    attenuation = m_albedo;
    return true;
}

/*
Vec3f Lambertian::random_in_unit_sphere()
{
    Vec3f p;
    do
    {
        p = 2.0 * Vec3f( m_dist( m_gen ), m_dist( m_gen ), m_dist( m_gen ) ) - Vec3f( 1, 1, 1 );
    } while (p.MagnitudeSquared() >= 1.0);
    return p;
} */