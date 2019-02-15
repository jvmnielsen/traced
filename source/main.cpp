
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
    auto cube1 = parser.GetMeshFromFile("../assets/cube.obj"); // NOTE: windows and unix paths differ
    auto cube2 = parser.GetMeshFromFile("../assets/cube.obj");
    auto lightSource = parser.GetMeshFromFile("../assets/plane.obj");
    auto floor = parser.GetMeshFromFile("../assets/plane.obj");
    auto rightWall = parser.GetMeshFromFile("../assets/plane.obj");
    auto leftWall = parser.GetMeshFromFile("../assets/plane.obj");
    auto backWall = parser.GetMeshFromFile("../assets/plane.obj");
    auto ceiling = parser.GetMeshFromFile("../assets/plane.obj");

    auto cube1Transform = std::make_unique<Transform>();
    cube1Transform->Translate({-3.f, -4.1f, -3.0f});
    cube1Transform->Rotate({0.0, 1.0, 0.0}, 20.0);
    cube1Transform->Scale({6.0, 12.0, 6.0});
    cube1->TransformBy(std::move(cube1Transform));

    auto cube2Transform = std::make_unique<Transform>();
    cube2Transform->Translate({4.f, -6.1f, 2.f});
    cube2Transform->Rotate({0.0f, 1.0f, 0.0f}, 70.0f);
    cube2Transform->Scale({5.5f, 8.0f, 5.5f});
    cube2->TransformBy(std::move(cube2Transform));


    auto lightTransform = std::make_unique<Transform>();
    lightTransform->Translate({0.0f, 9.9f, 0.0f});
    lightTransform->Rotate({0.0f, .0f, 1.0f}, 180.0f);
    lightTransform->Scale({3.17f, 3.17f, 3.17f});
    lightSource->TransformBy(std::move(lightTransform));
    
    auto floorTransform = std::make_unique<Transform>();
    floorTransform->Translate({0.0f, -10.0f, 0.0f});
    floorTransform->Scale({10.0f, 10.0f, 10.0f});
    floor->TransformBy(std::move(floorTransform));


    auto rightTransform = std::make_unique<Transform>();
    rightTransform->Translate({10.f, 0.0f, 0.0f});
    rightTransform->Rotate({0.0f, 0.0f, 1.0f}, 90.0f);
    rightTransform->Scale({10.0f, 10.0f, 10.0f});
    rightWall->TransformBy(std::move(rightTransform));

    auto leftTransform = std::make_unique<Transform>();
    leftTransform->Translate({-10.0f, 0.0f, 0.0f});
    leftTransform->Rotate({0.0f, 0.0f, 1.0f}, -90.0f);
    leftTransform->Scale({10.0f, 10.0f, 10.0f});
    leftWall->TransformBy(std::move(leftTransform));

    auto backTransform = std::make_unique<Transform>();
    backTransform->Translate({0.0f, 0.f, -10.0f});
    backTransform->Rotate({1.0f, 0.0f, 0.0f}, 90.0f);
    backTransform->Scale({10.0f, 10.0f, 10.0f});
    backWall->TransformBy(std::move(backTransform));

    auto ceilingTransform = std::make_unique<Transform>();
    ceilingTransform->Translate({0.0f, 10.0f, 0.0f});
    ceilingTransform->Rotate({0.0f, 0.0f, 1.0f}, 180.0f);
    ceilingTransform->Scale({10.0f, 10.0f, 10.0f});
    ceiling->TransformBy(std::move(ceilingTransform));


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
    std::vector<std::unique_ptr<Mesh>> lights;
    //meshes.push_back(std::move(cube1));
    //meshes.push_back(std::move(cube2));
    meshes.push_back(std::move(floor));
    //meshes.push_back(std::move(rightWall));
    //meshes.push_back(std::move(leftWall));
    //meshes.push_back(std::move(backWall));
    //meshes.push_back(std::move(ceiling));

    lights.push_back(std::move(lightSource));

    auto scene = std::make_unique<Scene>(std::move(meshes), std::move(lights));
    scene->SetBackgroundColor(Color3f{0.0f});

    auto camera = std::make_unique<Camera>(Point3f(0.0f, 0.0f, 40.0f), Point3f(0.0f,0.0f,-1.0f),
                             Vec3f(0.0f,1.0f,0.0f), 40.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    auto window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    auto buffer = std::make_shared<ImageBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    Renderer renderer{ std::move(camera), std::move(scene), buffer };
    std::thread RenderThread{ &Renderer::Render, std::ref(renderer), 10 };
    std::cout << "Render-thread started\n";

    window->InitializeWindow(*buffer);
    window->CheckForInput(*buffer);

    RenderThread.join();

    return 0;
}