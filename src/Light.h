#pragma once
#include "MathUtil.h"
#include "SDL_stdinc.h"

struct LightingInfo
{
	Vecf direction;
	Vecf intensity;
	float distance;
};

class Light
{
public:
    explicit Light(const Vecf& color, const float intensity = 1);

    virtual ~Light() = default;
	virtual void illuminate(const Vecf& point, LightingInfo& info) const = 0;

    Vecf m_color;
    float m_intensity;

};

class DistantLight : public Light
{
public:
	DistantLight(const Vecf& color, const float intensity, const Vecf& direction)
		: Light(color, intensity)
		, m_direction(direction)
	{}

	void illuminate(const Vecf& point, LightingInfo& info) const override
	{
		info.direction = m_direction;
		info.intensity = m_color * m_intensity;
		info.distance = -1;
	}

	Vecf m_direction;
};

class PointLight : public Light
{
public:
	PointLight(const Vecf& color, const float intensity, const Vecf& position)
		: Light(color, intensity)
		, m_position(position)
	{}

	void illuminate(const Vecf& point, LightingInfo& info) const override
	{
		info.direction = point - m_position;
		const auto r2 = info.direction.MagnitudeSquared();
		info.distance = sqrtf(r2);
		info.direction /= info.distance;
		info.intensity = m_intensity * m_color / (4 * M_PI * r2);
	}

	Vecf m_position;
};