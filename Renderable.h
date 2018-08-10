#pragma once
#include "Ray.h"

class Material;

class Renderable
{
public:
    //virtual ~Renderable() = default;
    virtual bool is_hit_by( const Rayf& ray, float& t ) const = 0;

    Vec3f m_surface_color = { 150, 20, 100 };
};

