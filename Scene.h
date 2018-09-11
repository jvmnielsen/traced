#pragma once
#include <vector>
#include "RenderPrimitive.h"
#include "Pixel.h"
#include "Ray.h"
#include <random>
#include "PixelBuffer.h"
#include <memory>
#include <limits>
#include "HitData.h"
#include "Window.h"


class Scene
{
public:
    Scene() {}
    Scene( //Pixel& background_color,
            const unsigned int screen_width,
            const unsigned int screen_height ) 
        //: m_background_color( background_color )
        : m_screen_width( screen_width )
        , m_screen_height( screen_height )
        , m_gen( std::random_device()() )
        , m_dist( 0, 1 )
    {
		
	}

    //void add_object_to_scene( RenderPrimitive& hitable );
    void render(PixelBuffer& buffer);

	void add_object_to_scene(const std::shared_ptr<RenderPrimitive>& render_ptr);

    

    unsigned int m_screen_width;
    unsigned int m_screen_height;


private:
    
    Pixel m_background_color;
    
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dist;

	std::vector<std::shared_ptr<RenderPrimitive>> m_scene_objects;

    const float m_infinity = std::numeric_limits<float>::max();

    HitData trace( const Rayf& ray );
	
    Vec3f cast_ray( const Rayf& ray );

	void load_objects_from_file(const std::string& file_name);
};

