#pragma once
//#include "Shape.h"
#include "Material.h"
//#include "Shape.h"

class Triangle;
//class Shape;
//class Material;

class Intersection
{
public:

    
    Intersection() {}
    Intersection(Point3f point, Normal3f geometricNormal);
   

    void Update(const Point3f& point, const Point2f& uvCoord, const Normal3f& geometricNormal, const Normal3f& shadingNormal, Triangle* shape);

    bool HasBeenHit() const;
    const Point3f& GetPoint() const;
    const Point2f& GetUV() const;
    Triangle* GetTriangle() const;
    const Normal3f& GetGeometricNormal() const;
    const Normal3f& GetShadingNormal() const;
    
    Material* GetMaterial() const;
    auto SetMaterial(Material* material) -> void { m_material = material; }
    //auto SetParentMeshMaterial(Material* material) -> void;

    Point3f OffsetShadingPoint() const;
    Point3f OffsetGeometricPoint() const;

    Color3f CalculateEmitted() const;

private:
    bool            m_hasBeenHit;
    Point3f         m_point;
    Point2f         m_uv;
    Triangle*       m_triangle;
    Normal3f        m_geometricNormal;
    Normal3f        m_shadingNormal;
    Material*       m_material;
};