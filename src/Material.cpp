#include "Material.h"
#include "MathUtil.h"
#include "VisibilityTester.h"
#include "BSDF.h"

bool refract(const Vec3f& vec, const Vec3f& n, float ni_over_nt, Vec3f& refracted)
{
    Vec3f norm = Normalize(vec);
    float dt = norm.DotProduct(n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted =  (norm - n * dt) - n * sqrt(discriminant) * ni_over_nt ;
        return true;
    }
    else
        return false;
}

float Schlick(float cosine, float refractiveIndex)
{
    float r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}

Vec3f Material::RandomInUnitSphere()
{
    Vec3f randomVec;
    do
    {
        randomVec = 2.0f * Point3f{ m_dist(m_gen), m_dist(m_gen), m_dist(m_gen) } - Point3f{ 1.0f, 1.0f, 1.0f };
    } while (randomVec.LengthSquared() >= 1.0f);

    return randomVec;
}

bool Matte::Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered)
{
    const auto target = isect.m_point + isect.m_normal + RandomInUnitSphere(); // Move point into unit-sphere by shifting with normal, then move with random vec
    scattered = Rayf{isect.m_point, target - isect.m_point};
    attenuation = m_diffuse;
    return true;
}

Vec3f Reflect(const Vec3f& vec, const Vec3f& n)
{
    return vec - 2 * vec.DotProduct(n) * n;
}

bool Metal::Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered)
{
    const auto reflected = Reflect(rayIn.Direction(), isect.m_normal);
    scattered = Rayf{isect.m_point, reflected + m_fuzz * RandomInUnitSphere()};
    attenuation = m_albedo;
    return scattered.Direction().DotProduct(isect.m_normal) > 0;
}

bool Dielectric::Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered)
{
    Vec3f outward_normal;
    const auto reflected = Reflect(rayIn.Direction(), isect.m_normal);
    float ni_over_nt;
    attenuation = Color3f(1.0f);
    Vec3f refracted;
    float reflect_prob;
    float cosine;

    if (rayIn.Direction().DotProduct(isect.m_normal) > 0) {
        outward_normal = -isect.m_normal;
        ni_over_nt = m_refractiveIndex;
        cosine = m_refractiveIndex * rayIn.Direction().DotProduct(isect.m_normal) / rayIn.Direction().Length();
    }
    else {
        outward_normal = isect.m_normal;
        ni_over_nt = 1.0 / m_refractiveIndex;
        cosine = -rayIn.Direction().DotProduct(isect.m_normal) / rayIn.Direction().Length();
    }
    if (refract(rayIn.Direction(), outward_normal, ni_over_nt, refracted)) {
        reflect_prob = Schlick(cosine, m_refractiveIndex);
    }
    else {
        scattered = Rayf(isect.m_point, reflected);
        reflect_prob = 1.0;
    }
    if (m_dist(m_gen) < reflect_prob) {
        scattered = Rayf(isect.m_point, reflected);
    }
    else {
        scattered = Rayf(isect.m_point, refracted);
    }
    return true;
}
