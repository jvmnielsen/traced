#pragma once
#include "Vec3.h"

class Light
{
public:
    explicit Light(const Vec3f& color, const float intensity = 1);

    virtual ~Light() = default;
    virtual void illuminate() const = 0;

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

	void illuminate() const override
	{
		
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

	void illuminate() const override
	{

	}

	Vec3f m_position;
};