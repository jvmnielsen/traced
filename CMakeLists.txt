cmake_minimum_required(VERSION 3.12)

project(Traced)

add_subdirectory(src)
# add_subdirectory(extern)

include(FetchContent)
FetchContent_Declare(
  graphics-math
  GIT_REPOSITORY https://github.com/jvmnielsen/graphics-math.git
  GIT_TAG dev
)
FetchContent_MakeAvailable(graphics-math)

FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/SDL-mirror/SDL
  GIT_TAG master
)
FetchContent_MakeAvailable(SDL)


# set(CMAKE_CXX_STANDARD 17)

# include_directories(${CMAKE_SOURCE_DIR}/third_party/SDL2/include)

# file(GLOB LIBRARIES "third_party/SDL2/lib/*.dylib")

# set(SOURCE_FILES
#         source/acceleration/bvh.hpp source/acceleration/bvh.cpp
#         source/acceleration/aabb.hpp source/acceleration/aabb.cpp
#         source/acceleration/bounds.hpp source/acceleration/bounds.cpp

#         source/core/intersection.hpp source/core/intersection.cpp
#         source/core/renderer.hpp source/core/renderer.cpp
#         source/core/sampler.hpp source/core/sampler.cpp
#         source/core/scene.hpp source/core/scene.cpp

#         source/geometry/mesh.hpp source/geometry/mesh.cpp
#         source/geometry/triangle.hpp source/geometry/triangle.cpp

#         source/imaging/camera.hpp source/imaging/camera.cpp
#         source/imaging/color3.hpp
#         source/imaging/image_buffer.hpp source/imaging/image_buffer.cpp
#         source/material/emissive.hpp

#         source/material/material.hpp source/material/material.cpp

#         source/math/math_util.hpp source/math/math_util.cpp
#         source/math/matrix4x4.hpp
#         source/math/normal3.hpp
#         source/math/point2.hpp
#         source/math/point3.hpp
#         source/math/ray.hpp
#         source/math/transform.hpp source/math/transform.cpp
#         source/math/vec2.hpp
#         source/math/vec3.hpp
#         source/math/onb.hpp
#         source/math/onb.cpp

#         source/utility/parser.hpp source/utility/parser.cpp
#         source/utility/utility.hpp
#         source/utility/window.hpp source/utility/window.cpp

#         source/main.cpp source/material/emissive.cpp source/material/distributions.hpp source/material/distributions.cpp)



# add_executable(Traced ${SOURCE_FILES})
# target_link_libraries(Traced ${LIBRARIES})

