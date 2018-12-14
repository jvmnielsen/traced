#include "material.hpp"
#include "../math/normal3.hpp"
//#include "MathUtil.h"

auto
Matte::ComputeScatteringFunctions(Intersection& isect) -> void {
    //isect.m_bsdf = &m_lambertian;

    std::vector<std::unique_ptr<BxDF>> bxdfs;
    bxdfs.emplace_back(std::make_unique<Lambertian>(m_albedo));

    isect.m_bsdf = std::make_unique<BSDF>(isect, std::move(bxdfs));
}

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
        //randomVec = 2.0f * Point3f{ m_dist(m_gen), m_dist(m_gen), m_dist(m_gen) } - Point3f{ 1.0f, 1.0f, 1.0f };
    } while (randomVec.LengthSquared() >= 1.0f);

    return randomVec;
}

/*
bool Matte::Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered)
{
    const auto target = isect.m_point + isect.m_geometricNormal + RandomInUnitSphere(); // Move point into unit-sphere by shifting with normal, then move with random vec
    scattered = Rayf{isect.m_point, target - isect.m_point};
    attenuation = m_albedo;
    return true;
}

Vec3f Reflect(const Vec3f& vec, const Vec3f& n)
{
    return vec - 2 * vec.DotProduct(n) * n;
}

bool Metal::Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered)
{
    const auto reflected = Reflect(rayIn.GetDirection(), isect.m_geometricNormal);
    scattered = Rayf{isect.m_point, reflected + m_fuzz * RandomInUnitSphere()};
    attenuation = m_albedo;
    return scattered.GetDirection().DotProduct(isect.m_geometricNormal) > 0;
}

bool Dielectric::Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered)
{
    Vec3f outward_normal;
    const auto reflected = Reflect(rayIn.GetDirection(), isect.m_geometricNormal);
    float ni_over_nt;
    attenuation = Color3f(1.0f);
    Vec3f refracted;
    float reflect_prob;
    float cosine;

    if (rayIn.GetDirection().DotProduct(isect.m_geometricNormal) > 0) {
        outward_normal = -isect.m_geometricNormal;
        ni_over_nt = m_refractiveIndex;
        cosine = m_refractiveIndex * rayIn.GetDirection().DotProduct(isect.m_geometricNormal) / rayIn.GetDirection().Length();
    }
    else {
        outward_normal = isect.m_geometricNormal;
        ni_over_nt = 1.0 / m_refractiveIndex;
        cosine = -rayIn.GetDirection().DotProduct(isect.m_geometricNormal) / rayIn.GetDirection().Length();
    }
    if (refract(rayIn.GetDirection(), outward_normal, ni_over_nt, refracted)) {
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
} */
