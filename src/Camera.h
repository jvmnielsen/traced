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
    Camera(const Vec3f& look_from, const Vec3f& look_at, const Vec3f& v_up, const float v_fov, const float aspect);
    
    Rayf GetRay(const float u, const float v) const;

    Vec3f origin;
    Vec3f lowerLeftCorner;
    Vec3f horizontal;
    Vec3f vertical;
};

