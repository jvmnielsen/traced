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
    /*
    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    //auto scene = std::make_unique<Scene>();
    Scene scene;

    auto camera = std::make_unique<Camera>(Vec3f(0.0f, 2.5f, 0.0f), Vec3f(0.0f,0.0f,-10.0f), Vec3f(0.0f,1.0f,0.0f), 45.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    scene.SetCamera(std::move(camera));

    auto lightOne = std::make_unique<PointLight>(Color3f(0.6, 0.2f, 0.8f), 900.0f, Vec3f(-2.2f, 4.0f, -2.5f));
    scene.AddLight(std::move(lightOne));

    //auto lightTwo = std::make_unique<PointLight>(Vecf(0.533f, 0.8f, 0.6f), 40.0f, Vecf(-2, 2, -2));
    //scene.AddLight(std::move(lightTwo));


    scene.SetBackgroundColor({0.0f, 0.0f, 0.0f});

    //ConstructStandardBox(scene);

    Parser parser;

    auto plane = parser.Parse("assets/plane.obj");
    auto leftPlane = plane->CloneMesh();
    auto rightPlane = plane->CloneMesh();
    auto backPlane = plane->CloneMesh();

    plane->ScaleBy(6.0f);
    plane->TranslateBy({0.0f,-0.5f,-9.0f});
    plane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(plane));

    leftPlane->RotateAroundZ(80.0f);
    leftPlane->ScaleBy(5.0f);
    leftPlane->TranslateBy({-5.0f,0.0f,-9.0f});
    leftPlane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(leftPlane));

    rightPlane->RotateAroundZ(-90.0f);
    rightPlane->ScaleBy(5.0f);
    rightPlane->TranslateBy({5.0f,2.0f,-9.0f});
    rightPlane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(rightPlane));

    backPlane->RotateAroundX(50.0f);
    backPlane->ScaleBy(5.0f);
    backPlane->TranslateBy({0.0f,10.0f,-14.0f});
    backPlane->SetMaterialType(Diffuse);
    scene.AddMesh(std::move(backPlane)); */


    /*
    auto teapot = parser.Parse("assets/teapot.obj");

    
    teapot->SetMaterialType(Diffuse);
    teapot->RotateAroundY(30.0f);
    teapot->ScaleBy(0.08f);
    teapot->TranslateBy({0,0.8f,-7});
    scene.AddMesh(std::move(teapot));  */
    

    /*
    auto sphereOne = std::make_unique<Sphere>(Vecf(0.1f, 0.50f,-7.0f), 0.5f, Vecf(0.18f), ReflectAndRefract);
    scene.AddRenderable(std::move(sphereOne));
    auto sphereTwo = std::make_unique<Sphere>(Vecf(-0.5f, 0.5f, -7.6f), 0.5f, Vecf(0.18f), Reflective);
    scene.AddRenderable(std::move(sphereTwo));
    auto sphereThree = std::make_unique<Sphere>(Vecf(0.7f, 0.5f, -6.2f), 0.50f, Vecf(0.18f), Diffuse);
    scene.AddRenderable(std::move(sphereThree)); */

    /*
    Window window = {SCREEN_WIDTH, SCREEN_HEIGHT};
    ImageBuffer buffer = {SCREEN_WIDTH, SCREEN_HEIGHT};
    //auto window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    //auto buffer = std::make_unique<ImageBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);

    std::thread RenderThread{ &Scene::Render, std::ref(scene), std::ref(buffer) };
    //scene.Render(buffer);

    window.InitializeWindow(buffer);
    window.CheckForInput(buffer);

    RenderThread.join(); */
    
    //ThreadManager manager{ std::move(window), std::move(buffer) };
    //manager.Run(*scene); */

    constexpr std::array<std::array<float, 4>, 4> m = {
            2, 2, 3, 1,
            9, 82, 9, 9,
            0, 1, 92, 0,
            0, 1, 0, 0
    };

    constexpr Matrix4x4f test { m };
    test.PrettyPrint();

    Vec3f vec { 2.0f, 9.0f, 81.0f };
    Vec3f vec2 { 3.0f, 9.0f, 81.0f };
    //constexpr auto vec3 = vec + vec2;

    Math::Swap(vec, vec2);

    vec.PrettyPrint();
    vec2.PrettyPrint();

    std::cout << vec.x << std::endl;
    std::cout << vec2.x << std::endl;

    constexpr Matrix4x4f test2 = test.Invert();
    test2.PrettyPrint();

    return 0;
}