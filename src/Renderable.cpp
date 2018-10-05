#include "Renderable.h"


void HitData::set_normal(const Rayf& ray)
{
    m_ptr->set_normal(*this);
}

void HitData::update_closest(Renderable* ptr, const Rayf& ray)
{
    if (ptr->m_material == ReflectAndRefract && ray.m_rayType == ShadowRay) // we skip in this case
        return;

    if (m_t < m_t_closest && m_t > 0)
    {

        m_barycentric_closest = m_barycentric_coord;
        m_t_closest = m_t;
        //m_point_closest = m_point;
        m_ptr = ptr;
        m_point = ray.point_at_parameter(m_t_closest);
        m_has_been_hit = true; // optimise, is done every time
    }

}