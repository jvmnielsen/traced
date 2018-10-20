#include "Material.h"
#include "MathUtil.h"
#include "VisibilityTester.h"
#include "BSDF.h"

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


Color3f Matte::CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene) const
{
    Color3f color{ 0 };

    for (const auto& light : scene.m_lights)
    {
        VisibilityTester tester;
        if (tester.IsVisible(isect.m_point + isect.m_normal * 1e-4f, light->m_position, scene)) // careful with self-intersection
        {
            PointLightingInfo info;
            light->IlluminatePoint(isect.m_point, info);

            //const Vec3f offset = light->m_position - isect.m_point;
            //const float distanceToLight = offset.Length();
            //const Vec3f wi = Normalize(offset);
            const auto wo = -rayIn.Direction().Normalize();
            const auto wi = info.directionToLight;

            //const Color3f intensity = light->m_intensity / (4 * M_PI * distanceToLight * distanceToLight);

            Lambertian lm{ m_diffuse };
            color += info.lightIntensity * std::max(0.0f, wi.DotProduct(isect.m_normal)) *
                    lm.EvaluateFiniteScatteringDensity(wo, wi);
        }
    }

    return color;
}


Color3f Plastic::CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene) const
{
    Color3f color{ 0 };

    for (const auto& light : scene.m_lights)
    {
        VisibilityTester tester;
        if (tester.IsVisible(isect.m_point + isect.m_normal * 1e-4f, light->m_position, scene)) // careful with self-intersection
        {
            PointLightingInfo info;
            light->IlluminatePoint(isect.m_point, info);

            const auto wo = -rayIn.Direction().Normalize();
            const auto wi = info.directionToLight;

            Phong phong{ m_diffuse, m_specular, m_smoothness, isect.m_normal };
            color += info.lightIntensity * std::max(0.0f, wi.DotProduct(isect.m_normal)) *
                     phong.EvaluateFiniteScatteringDensity(wo, wi);
        }
    }

    return color;
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