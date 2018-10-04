#include "Material.h"
#include "MathUtil.h"

bool refract( const Vecf& v, const Vecf& n, float ni_over_nt, Vecf& refracted )
{
    Vecf uv = UnitVector(v);
    float dt = DotProduct(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt( discriminant );
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

Rayf Lambertian::scatter(const Rayf& ray, const HitData& hit_data) const
{
    /*
    Vec3f target = rec.m_point + rec.normal + random_in_unit_sphere();
    scattered = Rayf(rec.p, target - rec.p);
    attenuation = m_albedo;
    return true; */
    return {};
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