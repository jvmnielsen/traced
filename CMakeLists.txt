cmake_minimum_required(VERSION 3.12)

project(RayTracer CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(RayTracer src/main.cpp src/Camera.h src/Camera.cpp src/Controller.h src/Controller.cpp
        src/HitData.h src/Imaging.h src/Imaging.cpp src/Light.cpp src/Light.h src/Material.cpp
        src/Material.cpp src/MathUtil.cpp src/MathUtil.h src/Parser.cpp src/Parser.h src/Polygon.cpp
        src/Polygon.h src/PolygonMesh.cpp src/PolygonMesh.h src/Renderable.h src/Renderable.cpp
        src/Scene.cpp src/Scene.h src/Sphere.cpp src/Sphere.h src/Window.cpp src/Window.h)
SET_TARGET_PROPERTIES(RayTracer PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(RayTracer ${SDL2_LIBRARIES})