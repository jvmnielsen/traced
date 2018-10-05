cmake_minimum_required(VERSION 3.12)

project(RAYS CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${RAYS_SOURCE_DIR}/cmake")

set(SOURCE_FILES src/main.cpp src/Camera.h src/Camera.cpp src/Controller.h src/Controller.cpp
        src/HitData.h src/Imaging.h src/Imaging.cpp src/Light.cpp src/Light.h src/Material.cpp
        src/Material.cpp src/MathUtil.cpp src/MathUtil.h src/Parser.cpp src/Parser.h src/Polygon.cpp
        src/Polygon.h src/PolygonMesh.cpp src/PolygonMesh.h src/Renderable.h src/Renderable.cpp
        src/Scene.cpp src/Scene.h src/Sphere.cpp src/Sphere.h src/Window.cpp src/Window.h)
add_executable(RAYS ${SOURCE_FILES})

find_package(SDL2 REQUIRED)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})


target_link_libraries(RAYS ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})