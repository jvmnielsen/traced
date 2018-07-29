#include "Material.h"

Vec3f reflect(const Vec3f& v, const Vec3f& n)
{
	return v - 2 * dot(v, n) * n;
}
