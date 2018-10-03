#pragma once
#include <memory>
#include "Ray.h"

/*
class Renderable;

class HitData
{
public:
    HitData()
        : m_t(-1)
        , m_t_closest(std::numeric_limits<float>::max())
        , m_point(0)
        , m_ptr(nullptr)
        , m_has_been_hit(false)
    {}
    
    //~HitData();
    void update_closest(Renderable* ptr, const Rayf& ray);

    bool has_been_hit() const { return m_has_been_hit; }

	Vec3f point() const { return m_point; }

	Vec3f barycentric() const { return m_barycentric_closest; }

    Vec3f normal() const { return m_normal; }

    Renderable* ptr_to_rndrble() const { return m_ptr; }

    float m_t;
   
    Vec3f m_point;
	Vec3f m_barycentric_coord;
    Vec3f m_normal;
   
private:

    float m_t_closest;
    Vec3f m_barycentric_closest;
    Vec3f m_normal_closest;

    Renderable* m_ptr; // observing

    bool m_has_been_hit;
    
}; */

