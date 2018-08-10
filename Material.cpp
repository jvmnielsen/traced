#include "Material.h"

Vec3f reflect(const Vec3f& v, const Vec3f& n)
{
	return v - 2 * dot(v, n) * n;
}

bool refract( const Vec3f& v, const Vec3f& n, float ni_over_nt, Vec3f& refracted )
{
    Vec3f uv = unit_vector( v );
    float dt = dot( uv, n );
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

/*
Vec3f Lambertian::random_in_unit_sphere()
{
    Vec3f p;
    do
    {
        p = 2.0 * Vec3f( m_dist( m_gen ), m_dist( m_gen ), m_dist( m_gen ) ) - Vec3f( 1, 1, 1 );
    } while (p.length_squared() >= 1.0);
    return p;
} */