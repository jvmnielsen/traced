#include "Camera.h"

Vec3f random_in_unit_disk()
{
	// might be too expensive creating and destroying every call
	std::mt19937 generator{ std::random_device()() };
	std::uniform_real_distribution<> distribution(0, 1);

	Vec3f p;
	do {
		p = 2.0 * Vec3f(distribution(generator), distribution(generator), 0) - Vec3f(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

