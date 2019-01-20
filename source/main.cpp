
//#include "ThreadManager.h"

#include "utility/utility.hpp"
#include "utility/parser.hpp"
#include "core/scene.hpp"
#include "imaging/camera.hpp"
#include "utility/window.hpp"
#include "imaging/image_buffer.hpp"
#include "core/renderer.hpp"
#include <thread>
#include "math/transform.hpp"

// arguments necessary for SDL to be multi-platform
int main(int argc, char * argv[]) {

    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    Timer timer{std::string("test took ")};

    Parser parser;
    auto teapot = parser.GetMeshFromFile("assets/cube.obj"); // NOTE: windows and unix paths differ
    auto plane = parser.GetMeshFromFile("assets/plane.obj");
    auto floor = parser.GetMeshFromFile("assets/plane.obj");

    teapot->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 50.0f));
    //teapot->TransformBy(Transform::Scale({1.80f, 1.80f, 1.80f}));
    teapot->TransformBy(Transform::Translate({.0f, .0f, -3.0f}));

    plane->TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, -120.0f));
    plane->TransformBy(Transform::Scale({0.45f, 0.45f, 0.45f}));
    plane->TransformBy(Transform::Translate({-2.5f, 1.0f, -3.0f}));

    //floor.TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, 70.0f));
    floor->TransformBy(Transform::Scale({3.25f, 3.25f, 3.25f}));
    floor->TransformBy(Transform::Translate({0.0f, -0.5f, -3.0f}));

    auto matte = std::make_shared<Matte>();
    auto green = std::make_shared<Matte>();
    green->m_attenuation = Color3f{0.2, 0.3, 0.2};
    teapot->ApplyMaterial(green);
    floor->ApplyMaterial(matte);

    auto light = std::make_shared<Emissive>();
    plane->ApplyMaterial(light);


    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<Mesh> lights;
    meshes.push_back(std::move(teapot));
    meshes.push_back(std::move(floor));

    lights.push_back(*plane);

    auto scene = std::make_unique<Scene>(std::move(meshes), std::move(lights));
    scene->SetBackgroundColor(Color3f{0.0f});

    auto camera = std::make_unique<Camera>(Point3f(0.0f, 1.0f, 0.0f), Point3f(0.0f,0.0f,-3.0f),
                             Vec3f(0.0f,1.0f,0.0f), 68.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    auto window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    auto buffer = std::make_shared<ImageBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    Renderer renderer{ std::move(camera), std::move(scene), buffer };
    std::thread RenderThread{ &Renderer::Render, std::ref(renderer) };
    std::cout << "Render-thread started\n";

    window->InitializeWindow(*buffer);
    window->CheckForInput(*buffer);

    RenderThread.join();

    return 0;
}