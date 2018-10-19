#include <utility>

#pragma once
#include <memory>
#include "MathUtil.h"
#include "Intersection.h"

class Shape;
class Intersection;

class BoundingVolume
{
public:
    explicit BoundingVolume(
        const Point3f& lowerBound = Point3f{0},
        const Point3f& upperBound = Point3f{0})
    {
        m_bounds[0] = lowerBound;
        m_bounds[1] = upperBound;
    }

    ~BoundingVolume() = default;

    bool Intersects(const Rayf& ray, Intersection& isect);
    //bool IntersectsQuick(const Rayf& ray, Intersection& isect);

    void SetShape(std::shared_ptr<Shape> shape) { m_shape = std::move(shape); }

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect);
    std::shared_ptr<Shape> m_shape;
    Point3f m_bounds[2];
};

