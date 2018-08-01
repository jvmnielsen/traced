#pragma once
#include "Vec3.h"
#include "Ray.h"
#include <random>

#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

Vec3f random_in_unit_disk();


class Camera
{
public:
    Camera( Vec3f look_from, Vec3f look_at, Vec3f view_up, float vFOV, float aspect, float aperture, float focus_dist )
		: m_lens_radius( aperture / 2 )
		, m_origin( look_from )
	
	{
		m_w = unit_vector( look_from - look_at );
		m_u = unit_vector( cross( view_up, m_w ) );
		m_v = cross( m_w, m_u );

		const float PI = 3.14159265358979323846; // fix / move somewhere else

		float theta = vFOV *  PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		m_lower_left_corner = m_origin - half_width * focus_dist * m_u - half_height * focus_dist * m_v - focus_dist * m_w;
		m_horizontal = 2 * half_width * focus_dist * m_u;
		m_vertical = 2 * half_height * focus_dist * m_v;
	}
	

	Rayf get_ray( float s, float t ) 
	{
		Vec3f rd = m_lens_radius * random_in_unit_disk();
		Vec3f offset = m_u * rd.m_x + m_v * rd.m_y;
		return Rayf( m_origin + offset, m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset);
	}

	Vec3f m_u, m_v, m_w;

	Vec3f m_origin;
	Vec3f m_lower_left_corner;
	Vec3f m_horizontal;
	Vec3f m_vertical;
	float m_lens_radius;
};

