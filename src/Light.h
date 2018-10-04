#pragma once
#include "MathUtil.h"

struct LightingInfo
{
	Vec3f direction;
	Vec3f intensity;
	float distance;
};

class Light
{
public:
    explicit Light(const Vec3f& color, const float intensity = 1);

    virtual ~Light() = default;
	virtual void illuminate(const Vec3f& point, LightingInfo& info) const = 0;

    Vec3f m_color;
    float m_intensity;

};

class DistantLight : public Light
{
public:
	DistantLight(const Vec3f& color, const float intensity, const Vec3f& direction)
		: Light(color, intensity)
		, m_direction(direction)
	{}

	void illuminate(const Vec3f& point, LightingInfo& info) const override
	{
		info.direction = m_direction;
		info.intensity = m_color * m_intensity;
		info.distance = -1;
	}

	Vec3f m_direction;
};

class PointLight : public Light
{
public:
	PointLight(const Vec3f& color, const float intensity, const Vec3f& position)
		: Light(color, intensity)
		, m_position(position)
	{}

	void illuminate(const Vec3f& point, LightingInfo& info) const override
	{
		info.direction = point - m_position;
		const auto r2 = info.direction.MagnitudeSquared();
		info.distance = sqrtf(r2);
		info.direction /= info.distance;
		info.intensity = m_intensity * m_color / (4 * M_PI * r2);
	}

	Vec3f m_position;
};