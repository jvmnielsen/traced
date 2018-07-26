#pragma once
#include <vector>
#include "Hitable.h"
#include "Pixel.h"
#include "Ray.h"

class Scene
{
public:
    Scene() {}
    Scene( Pixel& background_color,
           unsigned int screen_width,
           unsigned int screen_height ) 
        : m_background_color( background_color )
        , m_screen_width( screen_width )
        , m_screen_height( screen_height )
    { }


    ~Scene();

    void add_object_to_scene( Hitable& hitable );
    void render();

private:
    std::vector<Hitable> m_scene_objects;
    Pixel m_background_color;
    unsigned int m_screen_width;
    unsigned int m_screen_height;

    bool intercepts( const Rayf& ray, const float t_min, const float t_max, hit_record& record );
    Vec3f color( const Rayf& ray );
};

