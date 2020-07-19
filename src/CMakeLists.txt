set(SOURCE_FILES
    acceleration/bvh.hpp 
    acceleration/bvh.cpp
    acceleration/aabb.hpp 
    acceleration/aabb.cpp
    acceleration/bounds.hpp 
    acceleration/bounds.cpp

    core/intersection.hpp 
    core/intersection.cpp
    core/renderer.hpp 
    core/renderer.cpp
    core/sampler.hpp 
    core/sampler.cpp
    core/scene.hpp 
    core/scene.cpp

    geometry/mesh.hpp 
    geometry/mesh.cpp
    geometry/triangle.hpp 
    geometry/triangle.cpp
    geometry/ray.hpp

    imaging/camera.hpp 
    imaging/camera.cpp
    imaging/image_buffer.hpp 
    imaging/image_buffer.cpp
    
    material/material.hpp 
    material/material.cpp

    utility/parser.hpp 
    utility/parser.cpp
    utility/utility.hpp
    utility/window.hpp 
    utility/window.cpp
)


add_executable(traced main.cpp ${SOURCE_FILES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(traced PRIVATE SDL2::SDL2main SDL2::SDL2-static graphics-math)