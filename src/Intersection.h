#pragma once
#include "MathUtil.h"
#include "Shape.h"

class Shape;
class Material;

class Intersection
{
public:


    explicit Intersection(
            bool hasBeenHit = false,
            const Point3f& point = Point3f{0},
            const Point2f& barycentric = Point2f{0},
            Shape* shape = nullptr,
            //BoundingVolume* boundingVolume = nullptr,
            const Normal3f& normal = Normal3f{0},
            //const Vec3f& wo = Vec3f{0},
            Material* material = nullptr)
        : m_hasBeenHit(hasBeenHit)
        , m_point(point)
        , m_uvCoord(barycentric)
        , m_shape(shape)
        //, m_boundingVolume(boundingVolume)
        , m_normal(normal)
        //, m_wo(wo)
        , m_material(material)
    {
    }

    //~Intersection() = default; 

    //void ComputeScatteringFunction(const Rayf& ray);

    inline void Update(const Point3f& point, const Point2f& uvCoord, Shape* shape, Material* material);

    inline bool HasBeenHit() const { return m_hasBeenHit; }
    inline const Point3f& GetPoint() const { return m_point; }
    inline const Point2f& GetUV() const { return m_uvCoord; }
    inline Shape* GetShape() const { return m_shape; }
    inline const Normal3f& GetNormal() const { return m_normal; }
    inline Material* GetMaterial() const { return m_material; }

private:
    bool            m_hasBeenHit;
    Point3f         m_point;
    Point2f         m_uvCoord;
    Shape*          m_shape;
    //BoundingVolume* m_boundingVolume;
    Normal3f        m_normal;
    //Vec3f           m_wo;
    Material*       m_material;
};