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
            const Vec2f& barycentric = Vec2f{0},
            Shape* shape = nullptr,
            //BoundingVolume* boundingVolume = nullptr,
            const Normal3f& normal = Normal3f{0},
            const Vec3f& wo = Vec3f{0},
            Material* material = nullptr)
            : m_hasBeenHit(hasBeenHit)
            , m_point(point)
            , m_barycentric(barycentric)
            , m_shape(shape)
            //, m_boundingVolume(boundingVolume)
            , m_normal(normal)
            , m_wo(wo)
            , m_matPtr(material)
    {
    } 

    ~Intersection() = default; 

    //void ComputeScatteringFunction(const Rayf& ray);

    bool            m_hasBeenHit;
    Point3f         m_point;
    Vec2f           m_barycentric;
    Shape*          m_shape;
    //BoundingVolume* m_boundingVolume;
    Normal3f        m_normal;
    Vec3f           m_wo;
    Material*       m_matPtr;
};