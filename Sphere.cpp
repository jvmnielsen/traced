#include "Sphere.h"

bool Sphere::hit( const Rayf& ray, const float t_min, const float t_max, hit_record& record ) const
{
    Vec3f oc = ray.origin() - m_center;
    float a = dot( ray.direction(), ray.direction() );
    float b = dot( oc, ray.direction() );
    float c = dot( oc, oc ) - m_radius * m_radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt( b*b - a * c )) / a;
        if (temp < t_max && temp > t_min)
        {
            record.t = temp;
            record.p = ray.point_at_parameter( record.t );
            record.normal = (record.p - m_center) / m_radius;
            return true;
        }
        temp = (-b + sqrt( b*b - a * c )) / a;
        if (temp < t_max && temp > t_min)
        {
            record.t = temp;
            record.p = ray.point_at_parameter( record.t );
            record.normal = (record.p - m_center) / m_radius;
            return true;
        }
    }
    return false;


}
