#pragma once
#include "Vec3.h"
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
    
    Rayf get_ray(const float u, const float v) const;

    Matrix44f look_at(const Vec3f& from, const Vec3f& to)
    {
        Vec3f tmp(0, 1, 0);

        const auto forward = normalize(from - to);

        const auto right = cross(tmp, forward);

        const auto up = cross(forward, right);

        Matrix44f cam_to_world;

        cam_to_world[0][0] = right.m_x;
        cam_to_world[0][1] = right.m_y;
        cam_to_world[0][2] = right.m_z;

        cam_to_world[1][0] = up.m_x;
        cam_to_world[1][1] = up.m_y;
        cam_to_world[1][2] = up.m_z;

        cam_to_world[2][0] = forward.m_x;
        cam_to_world[2][1] = forward.m_y;
        cam_to_world[2][2] = forward.m_z;
        
        cam_to_world[3][0] = from.m_x;
        cam_to_world[3][1] = from.m_y;
        cam_to_world[3][2] = from.m_z;

        return cam_to_world;
    }

    Vec3f m_origin;
    Vec3f m_lower_left_corner;
    Vec3f m_horizontal;
    Vec3f m_vertical;


    /*

    Vec3f random_in_unit_disk();

	
	

	Vec3f m_u, m_v, m_w;

	
	float m_lens_radius;

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dist; */
};

