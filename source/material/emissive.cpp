#include "emissive.hpp"


/*
auto
Light::Sample(const Intersection& ref, Normal3f& wi, float& pdf, Intersection& atLight) const -> Color3f {
    atLight = m_mesh->SampleSurface(pdf);
    wi = Normalize(atLight.GetPoint() - ref.GetPoint());
    // convert to solid angle
    pdf *= (ref.GetPoint() - atLight.GetPoint()).LengthSquared()
            / std::abs(Dot(atLight.GetGeometricNormal(), -wi));
    if(pdf == 0) return Color3f{0.0f};

    //return m_material->Emitted();
} */
