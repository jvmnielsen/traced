#pragma once
#include "Ray.h"

struct hit_record
{
    float t;
    Vec3f p;
    Vec3f normal;
};

class Hitable
{
public:
    virtual bool hit( const Rayf& ray, const float t_min, const float t_max, hit_record& record ) const = 0;
};

