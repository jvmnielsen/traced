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

    inline void Update(const Point3f& point, const Point2f& uvCoord, const Normal3f& geometricNormal, Shape* shape, Material* material);

    inline bool HasBeenHit() const;
    inline const Point3f& GetPoint() const;
    inline const Point2f& GetUV() const;
    inline Shape* GetShape() const;
    inline const Normal3f& GetGeometricNormal() const;
    inline const Normal3f& GetShadingNormal() const;
    inline Material* GetMaterial() const;

    inline Point3f OffsetShadingPoint() const;
    inline Point3f OffsetGeometricPoint() const;

    inline Color3f CalculateEmitted() const;

private:
    bool            m_hasBeenHit;
    Point3f         m_point;
    Point2f         m_uvCoord;
    Shape*          m_shape;
    Normal3f        m_geometricNormal;
    Normal3f        m_shadingNormal;
    Material*       m_material;
};