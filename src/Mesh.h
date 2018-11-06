#pragma once
#include <memory>
#include <vector>

#include <random>
//#include "Material.h"
//#include "BoundingVolume.h"
#include "Triangle.h"

//#include "BoundingVolume.h"
//#include "Triangle.h"
//#include "BoundingVolume.h"
//#include <iostream>

//#include "Material.h"

//class BoundingVolume;

class Mesh 
{
public:

    Mesh();
    ~Mesh();
    Mesh(std::shared_ptr<Material> material);
    Mesh(const Mesh& other);
    Mesh(Mesh&& other);

	auto Intersects(const Rayf& ray, Intersection& isect) -> bool;
    auto IntersectsFast(const Rayf& ray) const -> bool;

	//auto AddPolygon(Triangle& triangle) -> void;

    

    auto GetExtent() const -> std::array<Point3f, 2>;

    
    void TransformBy(const Transform& transform);

    //std::unique_ptr<Mesh> Clone(); // consider making ctor

    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;

};

