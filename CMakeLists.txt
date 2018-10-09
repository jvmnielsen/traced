cmake_minimum_required(VERSION 3.12)

project(RayTracer CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${CMAKE_SOURCE_DIR}/third_party/SDL2/include)

file(GLOB LIBRARIES "third_party/SDL2/lib/*.dylib")

set(SOURCE_FILES src/main.cpp src/ThreadManager.h src/ThreadManager.cpp
        src/Imaging.h src/Imaging.cpp src/Material.cpp
        src/Material.cpp src/MathUtil.cpp src/MathUtil.h src/Parser.cpp src/Parser.h src/Polygon.cpp
        src/Polygon.h src/Mesh.cpp src/Mesh.h src/Renderable.h src/Renderable.cpp
        src/Scene.cpp src/Scene.h src/Sphere.cpp src/Sphere.h src/Window.cpp src/Window.h)
add_executable(RayTracer ${SOURCE_FILES})
target_link_libraries(RayTracer ${LIBRARIES})

