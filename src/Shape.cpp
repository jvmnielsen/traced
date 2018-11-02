#include "Shape.h"

Shape::Shape()
    : m_material(nullptr)
    , m_gen(std::random_device()())
    , m_dist(0.0f, 1.0f) 
{}

Shape::Shape(std::shared_ptr<Material> material)
    : m_material(std::move(material))
    , m_gen(std::random_device()())
    , m_dist(0.0f, 1.0f) 
{}

const Material& Shape::GetMaterial() const { return *m_material; }
void Shape::SetMaterial(std::shared_ptr<Material> material) { m_material = material; }

/*
Point3f Shape::GetRandomPointOnSurface() const
{
    return GetPointOnSurface(m_dist(m_gen), m_dist(m_gen));
} */