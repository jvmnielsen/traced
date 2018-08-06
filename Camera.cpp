#include "Camera.h"

Vec3f Camera::random_in_unit_disk()
{
    Vec3f p;
	do {
		p = 2.0 * Vec3f( m_dist( m_gen ), m_dist( m_gen ), 0) - Vec3f(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

