#include "Window.h"
#include <vector>
#include "Imaging.h"
#include "MathUtil.h"
#include "Scene.h"
#include "Material.h"
#include "Sphere.h"
#include "ThreadManager.h"
#include "Parser.h"
#include <thread>
#include "Utility.h"

// arguments necessary for SDL to be multi-platform
int main(int argc, char * argv[])
{
    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    //auto scene = std::make_unique<Scene>();
    Scene scene;

    auto camera = std::make_unique<Camera>(Vecf(0.0f, 2.0f, -1.0f), Vecf(0.0f,0.0f,-3.0f), Vecf(0.0f,1.0f,0.0f), 90.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    scene.SetCamera(std::move(camera));

    auto lightOne = std::make_unique<PointLight>(Vecf(0.3556f, 0.533f, 0.9f), 600.0f, Vecf(2, 2, -2));
    scene.AddLight(std::move(lightOne));

    //auto lightTwo = std::make_unique<PointLight>(Vecf(0.533f, 0.8f, 0.6f), 40.0f, Vecf(-2, 2, -2));
    //scene.AddLight(std::move(lightTwo));


    scene.SetBackgroundColor({0.1f, 0.1f, 0.1f});

    Parser parser;
    auto plane = parser.Parse("assets/plane.obj");

    Matrix44f objectToWorld1 = Matrix44f(4, 0, 0, 0,
                                         0, 4, 0, 0,
                                         0, 0, 4, 0,
                                         0, 0.0f, -5.0f, 1);

    plane->TransformByMatrix(objectToWorld1);
    plane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane));


    /*
    auto teapot = parser.Parse("assets/teapot.obj");

    Matrix44f objectToWorld = Matrix44f(0.15, 0, 0, 0,
                                        0, 0.15, 0, 0,
                                        0, 0, 0.15, 0,
                                        0, 0.8f, -2.0f, 1);

    teapot->TransformByMatrix(objectToWorld);
    teapot->SetMaterialType(Reflective);
    scene.AddMesh(std::move(teapot));  */
    

    
    auto sphereOne = std::make_unique<Sphere>(Vecf(0.1f, 0.50f,-4.0f), 0.5f, Vecf(0.18f), ReflectAndRefract);
    scene.AddRenderable(std::move(sphereOne));
    auto sphereTwo = std::make_unique<Sphere>(Vecf(-0.5f, 0.5f, -4.6f), 0.5f, Vecf(0.18f), Reflective);
    scene.AddRenderable(std::move(sphereTwo));
    auto sphereThree = std::make_unique<Sphere>(Vecf(0.7f, 0.5f, -3.2f), 0.50f, Vecf(0.18f), Diffuse);
    scene.AddRenderable(std::move(sphereThree)); 

    Window window = {SCREEN_WIDTH, SCREEN_HEIGHT};
    ImageBuffer buffer = {SCREEN_WIDTH, SCREEN_HEIGHT};
    //auto window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    //auto buffer = std::make_unique<ImageBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);

    std::thread RenderThread{ &Scene::Render, std::ref(scene), std::ref(buffer) };
    //scene.Render(buffer);

    window.InitializeWindow(buffer);
    window.CheckForInput(buffer);

    RenderThread.join();
    
    //ThreadManager manager{ std::move(window), std::move(buffer) };
    //manager.Run(*scene);

    return 0;
}