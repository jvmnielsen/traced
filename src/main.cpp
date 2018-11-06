#include "Window.h"
#include <vector>
#include "Imaging.h"
#include "MathUtil.h"
#include "Sphere.h"
//#include "ThreadManager.h"
#include "Parser.h"
#include <thread>
#include "Renderer.h"
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
    /*
    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    auto scene = std::make_unique<Scene>();
    auto camera = std::make_unique<Camera>(Point3f(0.0f, 1.0f, 0.0f), Point3f(0.0f,.0f,-8.0f), Vec3f(0.0f,1.0f,0.0f), 60.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));

    //auto lightOne = std::make_unique<PointLight>(Color3f{200.0f}, Point3f(2.0f, 2.0f, -2.0f));
    //scene.AddLight(std::move(lightOne));

    auto green = std::make_shared<Matte>(Color3f{0.1f, 0.6f, 0.1f});
    auto red = std::make_shared<Matte>(Color3f{0.6f, 0.1f, 0.1f});
    auto white = std::make_shared<Matte>(Color3f{0.4f, 0.4f, 0.4f});
    //auto phong = std::make_shared<Plastic>(Color3f{ 0.8f, 0.0f, 0.0f}, Color3f(0.2f), 50.f);
    auto metal = std::make_shared<Metal>(Color3f{0.4f}, 0.4f);
    auto diel = std::make_shared<Dielectric>(1.5f);
    auto light = std::make_shared<DiffuseLight>();

    Parser parser;
    auto teapot = parser.Parse("assets/teapot.obj");
    auto plane = parser.Parse("assets/plane.obj");
    auto lightSquare = plane->Clone();
    
    auto rightPlane = plane->Clone();
    auto leftPlane = plane->Clone();
    auto backPlane = plane->Clone();
    auto topPlane = plane->Clone();

    plane->TransformBy(Transform::Scale({1.3f, 1.0f, 1.3f}));
    plane->TransformBy(Transform::Translate({0.0f, 0.0f, -3.0f}));
    plane->SetMaterial(red);
    //scene->AddShape(std::move(plane));
    
    lightSquare->TransformBy(Transform::Rotate({1.0f, 0.0f, 0.0f}, 180.0f));
    lightSquare->TransformBy(Transform::Scale({0.3f, 1.f, 0.3f}));
    lightSquare->TransformBy(Transform::Translate({0.3f, 1.7f, -2.2f}));
    lightSquare->SetMaterial(light);
    scene->AddAreaLight(std::move(lightSquare));

    teapot->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 42.0f));
    teapot->TransformBy(Transform::Scale({0.03f, 0.03f, 0.03f}));
    teapot->TransformBy(Transform::Translate({0.0f, 0.2f, -2.8f}));
    //teapot->m_material = metal;
    //scene.AddShape(teapot);

    
    auto sphere = std::make_unique<Sphere>(Point3f{-0.3f,0.3f,-2.8f}, 0.3f, red);
    scene->AddShape(std::move(sphere));

    auto sphere2 = std::make_shared<Sphere>(Point3f{0.1f,0.3f,-2.2f}, 0.3f, diel);
    //scene.AddShape(sphere2);


    scene->SetBackgroundColor(Color3f{0.0f});

    auto window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    auto buffer = std::make_shared<ImageBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    Renderer renderer{ std::move(camera), std::move(scene), buffer, nullptr };


    std::thread RenderThread{ &Renderer::Render, std::ref(renderer), 20 };

    window->InitializeWindow(*buffer);
    window->CheckForInput(*buffer);

    RenderThread.join();

    return 0; */
    {
    Timer timer{"Test"};

    Parser parser;
    auto teapot = parser.Parse("assets/teapot.obj");

    Scene scene;

    scene.m_meshes.push_back(std::move(teapot));

    }

    return 0;
}