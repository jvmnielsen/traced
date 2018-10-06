#pragma once
#include <vector>
#include "Renderable.h"
#include "Pixel.h"
#include <random>
#include "Imaging.h"
#include <memory>
#include <limits>
#include "Window.h"
#include "Light.h"
#include "Imaging.h"
#include "MathUtil.h"



class Scene
{
public:
	//Scene() = default; // : m_light(Vec3f(255, 255, 255), 10.0f, Vec3f(0, 0, -1)) {}


    Scene( 
            const unsigned int screen_width,
            const unsigned int screen_height ) 
        //: m_background_color( background_color )
        : m_screen_width( screen_width )
        , m_screen_height( screen_height )
        , m_gen( std::random_device()() )
        , m_dist( 0, 1 )
        , m_max_depth(5)
		//, m_light(Vec3f(200,120,210), 9.0f, Vec3f(0,0,-1))
    {
		
	}

    //void add_object_to_scene( Renderable& hitable );
    void Render(ImageBuffer &buffer);

	void add_object_to_scene(const std::shared_ptr<Renderable>& render_ptr);

    

    unsigned int m_screen_width;
    unsigned int m_screen_height;

	

	Vecf m_background_color;
private:

    uint32_t m_max_depth;


	std::vector<std::shared_ptr<Light>> m_scene_lights;
    
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
	
    Vecf CastRay(const Rayf& ray, uint32_t depth);

	void load_objects_from_file(const std::string& file_name);

	//DistantLight m_light;

    Vecf random_in_unit_sphere();

	float m_shadow_bias = 1e-4f;
};

