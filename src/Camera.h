#pragma once
#include "MathUtil.h"
#include "Ray.h"


#define _USE_MATH_DEFINES // for C++  

#include "Matrix44.h"


class Camera
{
public:

    Camera() = default;
    ~Camera() = default;

    Camera(float v_fov, float aspect);
    Camera(const Vecf& look_from, const Vecf& look_at, const Vecf& v_up, const float v_fov, const float aspect);
    
    Rayf GetRay(const float u, const float v) const;

    Vecf origin;
    Vecf lowerLeftCorner;
    Vecf horizontal;
    Vecf vertical;
};

