# add_subdirectory(acceleration)
# add_subdirectory(core)
# add_subdirectory(geometry)
# add_subdirectory(imaging)
# add_subdirectory(material)
# add_subdirectory(math)
# add_subdirectory(utility)

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

    # math/math_util.hpp 
    # math/math_util.cpp
    # math/matrix4x4.hpp
    # math/normal3.hpp
    # math/point2.hpp
    # math/point3.hpp
    # math/ray.hpp
    # math/transform.hpp 
    # math/transform.cpp
    # math/vec2.hpp
    # math/vec3.hpp
    # math/onb.hpp
    # math/onb.cpp

    utility/parser.hpp 
    utility/parser.cpp
    utility/utility.hpp
    utility/window.hpp 
    utility/window.cpp
)


add_executable(traced main.cpp ${SOURCE_FILES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(traced PRIVATE SDL2::SDL2main SDL2::SDL2-static graphics-math)