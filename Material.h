#pragma once

struct hit_record;

#include "Scene.h"
#include "Ray.h"
#include "Hitable.h"

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

Vec3f reflect(const Vec3f& v, const Vec3f& n);

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



