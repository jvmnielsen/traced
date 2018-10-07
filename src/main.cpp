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

// arguments necessary for SDL to be multi-platform
int main(int argc, char * argv[])
{
    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    //auto scene = std::make_unique<Scene>();
    Scene scene;

    auto lightOne = std::make_unique<PointLight>(Vecf(0.9f, 0.5f, 0.1f), 400.0f, Vecf(2, 2, -1));
    //auto lightTwo = std::make_unique<PointLight>(Vecf(0.9f, 0.5f, 0.1f), 400.0f, Vecf(2, 2, -1));
    scene.AddLight(std::move(lightOne));
    //scene->AddLight(lightTwo);


    scene.SetBackgroundColor({ 0.25f, 0.30f, 0.27f });

    Parser parser;
    auto plane = parser.parse("../assets/plane.obj");

    Matrix44f objectToWorld1 = Matrix44f(6, 0, 0, 0,
                                         0, 6, 0, 0,
                                         0, 0, 6, 0,
                                         0, 0.0f, -2.5f, 1);

    plane->TransformByMatrix(objectToWorld1);
    plane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane));

    auto teapot = parser.parse("../assets/teapot.obj");

    Matrix44f objectToWorld = Matrix44f(0.15f, 0, 0, 0,
                                        0, 0.15f, 0, 0,
                                        0, 0, 0.15f, 0,
                                        0, 0.8f, -2.0f, 1.0f);

    teapot->TransformByMatrix(objectToWorld);
    teapot->SetMaterialType(ReflectAndRefract);
    //scene.AddMesh(std::move(teapot));


    auto sphereOne = std::make_unique<Sphere>(Vecf(0.1f, 0.50f,-2.0f), 0.5f, Vecf(0.18f), ReflectAndRefract);
    auto sphereTwo = std::make_unique<Sphere>(Vecf(-0.5f, 0.5f, -3.0f), 0.5f, Vecf(0.18f), Diffuse);
    auto sphereThree = std::make_unique<Sphere>(Vecf(0.7f, 0.5f, -2.6f), 0.40f, Vecf(0.18f), Reflective);
    scene.AddRenderable(std::move(sphereOne));
    scene.AddRenderable(std::move(sphereTwo));
    scene.AddRenderable(std::move(sphereThree));

    Window window = { SCREEN_WIDTH, SCREEN_HEIGHT };
    ImageBuffer buffer = { SCREEN_WIDTH, SCREEN_HEIGHT };

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