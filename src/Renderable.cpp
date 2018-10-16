#include "Renderable.h"


void Intersection::CalculateNormal(const Rayf &ray)
{
    m_ptr->CalculateNormal(*this);
}

void Intersection::UpdateClosest(Renderable *ptr, const Rayf &ray)
{
    if (ptr->Material() == MaterialType::ReflectAndRefract && ray.getRayType() == RayType::ShadowRay) // we skip in this case
        return;

    if (m_t < m_t_closest && m_t > 0)
    {
        m_barycentric_closest = m_barycentric_coord;
        m_t_closest = m_t;
        //m_point_closest = m_point;
        m_ptr = ptr;
        m_point = ray.PointAtParameter(m_t_closest);
        m_has_been_hit = true; // optimise, is done every time
    }

}