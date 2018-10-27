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
#include "Renderer.h"

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

    Vec3f{1.0f} / 2;

    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    Scene scene;

    Camera camera { Point3f(0.0f, 1.0f, 0.0f), Point3f(0.0f,.0f,-8.0f), Vec3f(0.0f,1.0f,0.0f), 60.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT) };

    auto lightOne = std::make_unique<PointLight>(Color3f{200.0f}, Point3f(2.0f, 2.0f, -2.0f));
    scene.AddLight(std::move(lightOne));

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
    plane->m_material = white;
    scene.AddShape(plane);

    topPlane->TransformBy(Transform::Rotate({1.0f, 0.0f, 0.0f}, 180.0f));   
    topPlane->TransformBy(Transform::Translate({0.0f, 2.0f, -3.0f}));
    topPlane->m_material = white;
    scene.AddShape(topPlane);

    rightPlane->TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, 90.0f));
    rightPlane->TransformBy(Transform::Scale({1.8f, 1.0f, 1.3f}));
    rightPlane->TransformBy(Transform::Translate({1.f, 1.0f, -3.0f}));
    rightPlane->m_material = green;
    //scene.AddShape(rightPlane);

    leftPlane->TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, -90.0f));
    leftPlane->TransformBy(Transform::Scale({1.8f, 1.0f, 1.3f}));
    leftPlane->TransformBy(Transform::Translate({-1.f, 1.0f, -3.0f}));
    leftPlane->m_material = red;
    //scene.AddShape(leftPlane);

    backPlane->TransformBy(Transform::Rotate({1.0f, 0.0f, 0.0f}, 90.0f));
    backPlane->TransformBy(Transform::Scale({1.3f, 1.0f, 1.8f}));
    backPlane->TransformBy(Transform::Translate({0.0f, 1.0f, -3.85f}));
    backPlane->m_material = white;
    //scene.AddShape(backPlane);


    lightSquare->TransformBy(Transform::Rotate({1.0f, 0.0f, 0.0f}, 180.0f));
    lightSquare->TransformBy(Transform::Scale({0.3f, 1.f, 0.3f}));
    lightSquare->TransformBy(Transform::Translate({0.0f, 2.0f, -2.5f}));
    lightSquare->m_material = light;
    scene.AddShape(lightSquare);

   
    teapot->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 42.0f));
    teapot->TransformBy(Transform::Scale({0.03f, 0.03f, 0.03f}));
    teapot->TransformBy(Transform::Translate({0.0f, 0.2f, -2.8f}));
    teapot->m_material = metal;
    //scene.AddShape(teapot);

    
    auto sphere = std::make_shared<Sphere>(Point3f{-0.3f,0.3f,-2.8f}, 0.3f, metal);
    //scene.AddShape(sphere);

    auto sphere2 = std::make_shared<Sphere>(Point3f{0.1f,0.3f,-2.2f}, 0.3f, diel);
    //scene.AddShape(sphere2);


    scene.SetBackgroundColor(Color3f{0.0f});

    Window window = {SCREEN_WIDTH, SCREEN_HEIGHT};
    ImageBuffer buffer = {SCREEN_WIDTH, SCREEN_HEIGHT};
    StochasticRayTracer whit { 50, 1000 };


    std::thread RenderThread{ &StochasticRayTracer::Render, whit, std::ref(scene), std::ref(camera), std::ref(buffer) };

    window.InitializeWindow(buffer);
    window.CheckForInput(buffer);

    RenderThread.join();

    return 0;

}