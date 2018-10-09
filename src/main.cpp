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

    auto camera = std::make_unique<Camera>(Vecf(0.0f, 2.5f, 0.0f), Vecf(0.0f,0.0f,-5.0f), Vecf(0.0f,1.0f,0.0f), 60.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    scene.SetCamera(std::move(camera));

    auto lightOne = std::make_unique<PointLight>(Vecf(0.3556f, 0.533f, 0.9f), 300.0f, Vecf(0.9, 1.5, -1));
    scene.AddLight(std::move(lightOne));

    //auto lightTwo = std::make_unique<PointLight>(Vecf(0.533f, 0.8f, 0.6f), 40.0f, Vecf(-2, 2, -2));
    //scene.AddLight(std::move(lightTwo));


    scene.SetBackgroundColor({0.05f, 0.05f, 0.05f});

    Parser parser;
    auto plane = parser.Parse("../assets/plane.obj");
    auto plane2 = plane->CloneMesh();
    auto plane3 = plane->CloneMesh();
    auto plane4 = plane->CloneMesh();

    plane->ScaleBy(5.0f);
    plane->TranslateBy({0.0f,0,-5.0f});
    plane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane));

    plane2->RotateAroundZ(90.0f);
    plane2->ScaleBy(5.0f);
    plane2->TranslateBy({-5.0f,0,-5.0f});
    plane2->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane2));

    plane3->RotateAroundZ(-90.0f);
    plane3->ScaleBy(5.0f);
    plane3->TranslateBy({5.0f,0,-5.0f});
    plane3->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane3));

    plane4->RotateAroundX(-90.0f);
    plane4->ScaleBy(5.0f);
    plane4->TranslateBy({0.0f,0,-10.0f});
    plane4->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane4));

    /*
    auto teapot = parser.Parse("assets/teapot.obj");

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