#pragma once
#include <vector>
#include "Renderable.h"
#include "Pixel.h"
#include "Ray.h"
#include <random>
#include "PixelBuffer.h"
#include <memory>
#include <limits>
#include "HitData.h"
#include "Window.h"
#include "Light.h"


class Scene
{
public:
    Scene() : m_light(Vec3f(255, 255, 255), 10.0f, Vec3f(0, 0, -1)) {}


    Scene( //Pixel& background_color,
            const unsigned int screen_width,
            const unsigned int screen_height ) 
        //: m_background_color( background_color )
        : m_screen_width( screen_width )
        , m_screen_height( screen_height )
        , m_gen( std::random_device()() )
        , m_dist( 0, 1 )
		, m_light(Vec3f(200,100,200),14.0f,Vec3f(0,0,1))
    {
		
	}

    //void add_object_to_scene( Renderable& hitable );
    void render(PixelBuffer& buffer);

	void add_object_to_scene(const std::shared_ptr<Renderable>& render_ptr);

    

    unsigned int m_screen_width;
    unsigned int m_screen_height;

	


private:
    
    Pixel m_background_color;
    
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dist;

	std::vector<std::shared_ptr<Renderable>> m_simple_scene_objects;
    std::vector<std::shared_ptr<Renderable>> m_scene_meshes;

    const float m_infinity = std::numeric_limits<float>::max();

    HitData trace(const Rayf& ray);

    void trace_simple_objects(const Rayf& ray, HitData& hit_data);
    void trace_meshes(const Rayf& ray, HitData& hit_data);
	
    Vec3f cast_ray(const Rayf& ray);

	void load_objects_from_file(const std::string& file_name);

	DistantLight m_light;
};

