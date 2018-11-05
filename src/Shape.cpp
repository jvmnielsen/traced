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


auto
Shape::GetMaterial() const -> const Material&
{
    return *m_material;
}

auto
Shape::SetMaterial(std::shared_ptr<Material> material) -> void
{
    m_material = material;
}

/*
Point3f Shape::GetRandomPointOnSurface() const
{
    return GetPointOnSurface(m_dist(m_gen), m_dist(m_gen));
} */