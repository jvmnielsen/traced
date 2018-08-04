#pragma once
#include <vector>
#include "Hitable.h"
#include "Pixel.h"
#include "Ray.h"
#include <random>
#include "PixelBuffer.h"

class Scene
{
public:
    Scene() {}
    Scene( //Pixel& background_color,
           unsigned int screen_width,
           unsigned int screen_height ) 
        //: m_background_color( background_color )
        : m_screen_width( screen_width )
        , m_screen_height( screen_height )
		, m_generator(std::random_device()())
		, m_distribution(0,1) 
    {
		
	}

    //void add_object_to_scene( Hitable& hitable );
    void render( PixelBuffer& buffer );

    static Vec3f random_in_unit_sphere();

    std::vector<Hitable*> m_scene_objects; // need to destroy objects

    unsigned int m_screen_width;
    unsigned int m_screen_height;


private:
    
    Pixel m_background_color;
    

	// to generate random numbers [0,1]
	//std::random_device m_seed;
	std::mt19937 m_generator;
	std::uniform_real_distribution<> m_distribution;

    bool intercepts( const Rayf& ray, const float t_min, const float t_max, hit_record& record );
	
    Vec3f color( const Rayf& ray, int depth );
};

