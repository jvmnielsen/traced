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
#include "Integrator.h"

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

    Scene scene;

    Camera camera { Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f,0.0f,-1.0f), Vec3f(0.0f,1.0f,0.0f), 60.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT) };

    auto lightOne = std::make_unique<PointLight>(Color3f{550.0f}, Point3f(0.0f, 2.0f, -2.0f));
    scene.AddLight(std::move(lightOne));

    auto lamb = std::make_shared<Matte>(Color3f{0.18f});


    Parser parser;
    auto teapot = parser.Parse("../assets/teapot.obj");

    //teapot->TransformBy(Transform::Rotate({1.0f, 0.0f, 0.0f}, 90.0f));
    teapot->TransformBy(Transform::Scale({0.1f, 0.1f, 0.1f}));
    teapot->TransformBy(Transform::Translate({0.0f, 0.0f, -6.0f}));
    teapot->m_material = lamb;
    auto teapotBounding = teapot->GetBoundingVolume();
    teapotBounding->SetShape(teapot);
    scene.AddBoundingVolume(teapotBounding);

    /*
    auto sphere = std::make_shared<Sphere>(Point3f{0.f,0.f,-6.0f}, 1.5f, Color3f{0.18f}, lamb);
    auto boundingSphere = sphere->GetBoundingVolume();
    boundingSphere->SetShape(sphere);
    scene.AddBoundingVolume(boundingSphere);
    */

    scene.SetBackgroundColor({0.02f, 0.02f, 0.02f});

    Window window = {SCREEN_WIDTH, SCREEN_HEIGHT};
    ImageBuffer buffer = {SCREEN_WIDTH, SCREEN_HEIGHT};
    WhittedRayTracer whit { 10 };


    std::thread RenderThread{ &WhittedRayTracer::Render, whit, std::ref(scene), std::ref(camera), std::ref(buffer) };

    window.InitializeWindow(buffer);
    window.CheckForInput(buffer);

    RenderThread.join();

    return 0;

}