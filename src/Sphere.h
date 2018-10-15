#pragma once
#include "Renderable.h"

class Sphere :
    public Renderable
{
public:
    Sphere(const Point3f& center, const float radius, const Color3f& albedo, MaterialType material)
        : Renderable(albedo, material)
        , m_center(center)
        , m_radius(radius)
        , m_radius_squared(radius * radius)
    {}

    bool Intersects(const Rayf &ray, Intersection &intersection) override;

    bool solve_quadratic(const float a, const float b, const float c, float& solutionOne, float& solutionTwo) const;

    void TransformByMatrix(const Matrix4x4f &object_to_world) override { }

    void CalculateNormal(Intersection &intersection) const override;

    void SetMaterialType(const MaterialType& type) override
    {
        m_material = type;
    }

    void TranslateBy(const Vec3f& dir) override
    {
        m_center.x += dir.x;
        m_center.y += dir.y;
        m_center.z += dir.z;
    } 

    void RotateAroundX(float dir) override {} // do nothing (our representation of a sphere cannot meaningfully be rotated)
    void RotateAroundY(float dir) override {}
    void RotateAroundZ(float dir) override {}

    void ScaleBy(float factor) override
    {
        m_radius *= factor;
    }


private:
    Point3f m_center;
    float m_radius;
    float m_radius_squared;
	//Material* m_material;
    
};

