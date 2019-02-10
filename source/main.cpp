
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

    const unsigned int SCREEN_WIDTH = 600;
    const unsigned int SCREEN_HEIGHT = 600;

    Timer timer{std::string("test took ")};

    Parser parser;
    auto cube1 = parser.GetMeshFromFile("assets/cube.obj"); // NOTE: windows and unix paths differ
    auto cube2 = std::make_unique<Mesh>(*cube1);
    auto lightSource = parser.GetMeshFromFile("assets/plane.obj");
    auto floor = parser.GetMeshFromFile("assets/plane.obj");
    auto rightWall = parser.GetMeshFromFile("assets/plane.obj");
    auto leftWall = parser.GetMeshFromFile("assets/plane.obj");
    auto backWall = std::make_unique<Mesh>(*leftWall);
    auto ceiling = std::make_unique<Mesh>(*leftWall);

    cube1->TransformBy(Transform::Scale({6.0f, 12.0f, 6.0f}));
    cube1->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 20.0f));
    cube1->TransformBy(Transform::Translate({-3.f, -4.1f, -3.0f}));

    cube2->TransformBy(Transform::Scale({5.5f, 8.0f, 5.5f}));
    cube2->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 70.0f));
    cube2->TransformBy(Transform::Translate({4.f, -6.1f, 2.f}));


    lightSource->TransformBy(Transform::Rotate({0.0f, .0f, 1.0f}, 180.0f));
    lightSource->TransformBy(Transform::Scale({3.17f, 3.17f, 3.17f}));
    lightSource->TransformBy(Transform::Translate({0.0f, 9.9f, 0.0f}));

    //floor.TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, 70.0f));
    floor->TransformBy(Transform::Scale({10.0f, 10.0f, 10.0f}));
    floor->TransformBy(Transform::Translate({0.0f, -10.0f, 0.0f}));

    rightWall->TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, 90.0f));
    rightWall->TransformBy(Transform::Scale({10.0f, 10.0f, 10.0f}));
    rightWall->TransformBy(Transform::Translate({10.f, 0.0f, 0.0f}));
    
    leftWall->TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, -90.0f));
    leftWall->TransformBy(Transform::Scale({10.0f, 10.0f, 10.0f}));
    leftWall->TransformBy(Transform::Translate({-10.0f, 0.0f, 0.0f}));

    backWall->TransformBy(Transform::Rotate({1.0f, 0.0f, 0.0f}, 90.0f));
    backWall->TransformBy(Transform::Scale({10.0f, 10.0f, 10.0f}));
    backWall->TransformBy(Transform::Translate({0.0f, 0.f, -10.0f}));

    ceiling->TransformBy(Transform::Rotate({0.0f, 0.0f, 1.0f}, 180.0f));
    ceiling->TransformBy(Transform::Scale({10.0f, 10.0f, 10.0f}));
    ceiling->TransformBy(Transform::Translate({0.0f, 10.0f, 0.0f}));


    auto matte = std::make_shared<Matte>();
    auto green = std::make_shared<Matte>();
    green->m_attenuation = Color3f{0.2, 0.7, 0.2};
    auto red = std::make_shared<Matte>();
    red->m_attenuation = Color3f{0.7, 0.2, 0.2};
    cube1->ApplyMaterial(matte);
    cube2->ApplyMaterial(matte);
    floor->ApplyMaterial(matte);
    rightWall->ApplyMaterial(red);
    leftWall->ApplyMaterial(green);
    backWall->ApplyMaterial(matte);
    ceiling->ApplyMaterial(matte);

    auto light = std::make_shared<Emissive>();
    lightSource->ApplyMaterial(light);


    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<Mesh> lights;
    meshes.push_back(std::move(cube1));
    meshes.push_back(std::move(cube2));
    meshes.push_back(std::move(floor));
    meshes.push_back(std::move(rightWall));
    meshes.push_back(std::move(leftWall));
    meshes.push_back(std::move(backWall));
    meshes.push_back(std::move(ceiling));

    lights.push_back(*lightSource);

    auto scene = std::make_unique<Scene>(std::move(meshes), std::move(lights));
    scene->SetBackgroundColor(Color3f{0.0f});

    auto camera = std::make_unique<Camera>(Point3f(0.0f, 0.0f, 40.0f), Point3f(0.0f,0.0f,-1.0f),
                             Vec3f(0.0f,1.0f,0.0f), 40.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    auto window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    auto buffer = std::make_shared<ImageBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    Renderer renderer{ std::move(camera), std::move(scene), buffer };
    std::thread RenderThread{ &Renderer::Render, std::ref(renderer), 2000 };
    std::cout << "Render-thread started\n";

    window->InitializeWindow(*buffer);
    window->CheckForInput(*buffer);

    RenderThread.join();

    return 0;
}