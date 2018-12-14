#pragma once
#include "../math/vec3.hpp"
#include "color3.hpp"
#include "../math/point3.hpp"
#include "../math/math_util.hpp"
#include "../geometry/mesh.hpp"


struct LightingAtPoint
{
    Vec3f directionToLight;
    Color3f intensityAtPoint;
};

class Light
{
public:
    Light(std::unique_ptr<Mesh> mesh);

    auto Sample(const Intersection& ref, Normal3f& wi, float& pdf, Intersection& atLight) const -> Color3f {
        auto isect = m_mesh->SampleSurface(pdf);
        if (pdf == 0.0f || (isect.GetPoint() - ref.GetPoint()).LengthSquared() == 0.0f) {
            pdf = 0;
            return Color3f{0.0f};
        }
        wi = Normalize(isect.GetPoint() - ref.GetPoint());
        return Dot(isect.GetGeometricNormal(), -wi) > 0 ? Color3f{0.3f} : Color3f{0.0f};
    }

    auto PdfLi(const Intersection& ref, const Vec3f& wi) const -> float {
        //return m_mesh->Pdf(ref, wi);
    }

    Color3f m_radiance;

protected:

    //std::shared_ptr<Emissive>   m_material;
    std::unique_ptr<Mesh>       m_mesh;
};

