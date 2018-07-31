#pragma once
#include "Vec3.h"
#include "Ray.h"

class Camera
{
public:
    Camera( Vec3f look_from, Vec3f look_at, Vec3f view_up, float vFOV, float aspect )
	{
        Vec3f u, v, w;

        float theta = vFOV * M_PI / 180;
        float half_height = tan( theta / 2 );
        float half_width = aspect * half_height;
        
        origin = look_from;
        w = unit_vector( look_from - look_at );
        u = unit_vector( cross( view_up, w ) );
        v = cross( w, u );

        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
	}
	

	Rayf get_ray( float u, float v ) 
	{
		return Rayf(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

	Vec3f origin;
	Vec3f lower_left_corner;
	Vec3f horizontal;
	Vec3f vertical;
};

