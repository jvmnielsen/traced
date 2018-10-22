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

    Vec3f{1.0f} / 2;

    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    Scene scene;

    Camera camera { Point3f(0.0f, 1.0f, 0.0f), Point3f(0.0f,.0f,-8.0f), Vec3f(0.0f,1.0f,0.0f), 60.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT) };

    auto lightOne = std::make_unique<PointLight>(Color3f{600.0f}, Point3f(3.0f, 2.0f, -1.0f));
    scene.AddLight(std::move(lightOne));

    auto lamb = std::make_shared<Matte>(Color3f{0.18f});
    auto phong = std::make_shared<Plastic>(Color3f{ 0.1f, 0.3f, 0.5f}, Color3f(0.1f, 0.1f, 0.0f), 70.f);
    auto metal = std::make_shared<Metal>();

    Parser parser;
    auto teapot = parser.Parse("assets/teapot.obj");
    auto plane = parser.Parse("assets/plane.obj");
    auto plane2 = plane->Clone();
    auto plane3 = plane->Clone();
    auto plane4 = plane->Clone();

    plane->TransformBy(Transform::Scale({1.3f, 1.0f, 1.3f}));
    plane->TransformBy(Transform::Translate({0.0f, 0.0f, -3.0f}));
    plane->m_material = phong;
    scene.AddShape(plane);

    plane2->TransformBy(Transform::Scale({2.0f, 1.0f, 1.3f}));
    plane2->TransformBy(Transform::Rotate({0,0,1}, -90));
    plane2->TransformBy(Transform::Translate({-1.3f, 0.0f, -1.7f}));
    plane2->m_material = phong;
    scene.AddShape(plane2);

    plane3->TransformBy(Transform::Rotate({0,0,1}, 90));
    plane3->TransformBy(Transform::Scale({4.0f, 1.0f, 4.0f}));
    plane3->TransformBy(Transform::Translate({2.6f, 0.0f, -4.3f}));
    plane3->m_material = phong;
    //scene.AddShape(plane3);

    plane4->TransformBy(Transform::Rotate({1,0,0}, 90));
    plane4->TransformBy(Transform::Scale({4.0f, 1.0f, 4.0f}));
    plane4->TransformBy(Transform::Translate({0.0f, 0.0f, -8.0f}));
    plane4->m_material = phong;
    //scene.AddShape(plane4);



    teapot->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 30.0f));
    teapot->TransformBy(Transform::Scale({0.1f, 0.1f, 0.1f}));
    teapot->TransformBy(Transform::Translate({0.0f, 0.7f, -5.0f}));
    teapot->m_material = lamb;
    auto teapotBounding = teapot->GetBoundingVolume();
    teapotBounding->SetShape(teapot);
    scene.AddBoundingVolume(teapotBounding);

    
    auto sphere = std::make_shared<Sphere>(Point3f{-1.f,0.5f,-3.0f}, 0.5f, metal);
    auto boundingSphere = sphere->GetBoundingVolume();
    boundingSphere->SetShape(sphere);
    //scene.AddBoundingVolume(boundingSphere);

    auto sphere2 = std::make_shared<Sphere>(Point3f{1.0f,0.5f,-3.0f}, 0.5f, phong);
    auto boundingSphere2 = sphere2->GetBoundingVolume();
    boundingSphere2->SetShape(sphere2);
    //scene.AddBoundingVolume(boundingSphere2);

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