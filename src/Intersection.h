#pragma once
//#include "Shape.h"
#include "Material.h"
//#include "Shape.h"

class Shape;
//class Material;

class Intersection
{
public:

    explicit Intersection(
            bool hasBeenHit = false,
            const Point3f& point = Point3f{0},
            const Point2f& barycentric = Point2f{0},
            Shape* shape = nullptr,
            const Normal3f& geometricNormal = Normal3f{0},
            const Normal3f& shadingNormal = Normal3f{0},
            Material* material = nullptr);

    Intersection(Point3f point, Normal3f geometricNormal);

    void Update(const Point3f& point, const Point2f& uvCoord, const Normal3f& geometricNormal, Shape* shape, Material* material);

    bool HasBeenHit() const;
    const Point3f& GetPoint() const;
    const Point2f& GetUV() const;
    Shape* GetShape() const;
    const Normal3f& GetGeometricNormal() const;
    const Normal3f& GetShadingNormal() const;
    
    Material* GetMaterial() const;
    auto SetParentMeshMaterial(Material* material) -> void;

    Point3f OffsetShadingPoint() const;
    Point3f OffsetGeometricPoint() const;

    Color3f CalculateEmitted() const;

private:
    bool            m_hasBeenHit;
    Point3f         m_point;
    Point2f         m_uvCoord;
    Shape*          m_shape;
    Normal3f        m_geometricNormal;
    Normal3f        m_shadingNormal;
    Material*       m_material;
};