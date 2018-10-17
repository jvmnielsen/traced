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
#include <array>
#include "Utility.h"

/*
void ConstructStandardBox(Scene& scene)
{
    Parser parser;
    auto plane = parser.Parse("assets/plane.obj");
    auto leftPlane = plane->CloneMesh();
    auto rightPlane = plane->CloneMesh();
    auto backPlane = plane->CloneMesh();

    plane->ScaleBy(5.0f);
    plane->TranslateBy({0.0f,0,-9.0f});
    plane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane));

    leftPlane->RotateAroundZ(90.0f);
    leftPlane->ScaleBy(5.0f);
    leftPlane->TranslateBy({-5.0f,2.0f,-9.0f});
    leftPlane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(leftPlane));

    rightPlane->RotateAroundZ(-90.0f);
    rightPlane->ScaleBy(5.0f);
    rightPlane->TranslateBy({5.0f,2.0f,-9.0f});
    rightPlane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(rightPlane));

    backPlane->RotateAroundX(-90.0f);
    backPlane->ScaleBy(5.0f);
    backPlane->TranslateBy({0.0f,2.0f,-14.0f});
    backPlane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(backPlane));
} */

// arguments necessary for SDL to be multi-platform
int main(int argc, char * argv[])
{

    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    //auto scene = std::make_unique<Scene>();
    Scene scene;

    auto camera = std::make_unique<Camera>(Point3f(0.0f, 2.0f, 0.0f), Point3f(0.0f,0.0f,-10.0f), Vec3f(0.0f,1.0f,0.0f), 45.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    scene.SetCamera(std::move(camera));

    auto lightOne = std::make_unique<PointLight>(Color3f(0.7f, 0.7f, 0.7f), 0.01f, Point3f(-2.2f, 2.0f, -1.5f));
    scene.AddLight(std::move(lightOne));

    auto lightTwo = std::make_unique<PointLight>(Color3f(0.533f, 0.8f, 0.6f), 0.01f, Point3f(2, 2, -4));
    //scene.AddLight(std::move(lightTwo));


    scene.SetBackgroundColor({0.02f, 0.02f, 0.02f});

    //ConstructStandardBox(scene);

    Parser parser;

    auto plane = parser.Parse("../assets/plane.obj");
    auto leftPlane = plane->CloneMesh();
    //auto rightPlane = plane->CloneMesh();
    //auto backPlane = plane->CloneMesh();



    plane->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 90.0f));
    plane->TransformBy(Transform::Scale({3.0f, 3.0f, 1.0f}));
    plane->TransformBy(Transform::Translate({-4.0f, 0.0f, -4.0f}));
    plane->SetMaterialType(MaterialType::Diffuse);
    //scene.AddMesh(std::move(plane));

    leftPlane->TransformBy(Transform::Scale({6.0f, 6.0f, 6.0f}));
    leftPlane->TransformBy(Transform::Translate({0.0f, 0.0f, -6.0f}));
    scene.AddMesh(std::move(leftPlane));


    auto teapot = parser.Parse("../assets/teapot.obj");


    teapot->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 56.0f));
    teapot->TransformBy(Transform::Scale({0.08f, 0.08f, 0.08f}));
    teapot->TransformBy(Transform::Translate({-0.4f, 0.5f, -5.0f}));
    teapot->SetMaterialType(MaterialType::Diffuse);
    scene.AddMesh(std::move(teapot));
    


    auto sphereOne = std::make_unique<Sphere>(Point3f(1.1f, 0.50f,-7.0f), 0.5f, Color3f{0.18f, 0.1f, 0.2f}, MaterialType::Reflective);
    scene.AddRenderable(std::move(sphereOne));
    auto sphereTwo = std::make_unique<Sphere>(Point3f(-2.5f, 0.5f, -7.0f), 0.5f, Color3f{0.18f}, MaterialType::Reflective);
    scene.AddRenderable(std::move(sphereTwo));
    auto sphereThree = std::make_unique<Sphere>(Point3f(1.7f, 0.5f, -6.2f), 0.50f, Color3f{0.18f, 0.3f, 0.1f}, MaterialType::Diffuse);
    scene.AddRenderable(std::move(sphereThree));
    auto sphereThee = std::make_unique<Sphere>(Point3f(-0.7f, 0.5f, -4.2f), 0.50f, Color3f{0.18f, 0.09f, 0.2f}, MaterialType::Diffuse);
    //scene.AddRenderable(std::move(sphereThee));


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

}