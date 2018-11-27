
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
    auto teapot = parser.GetMeshFromFile("../assets/teapot.obj");
    auto teapot2 = std::make_unique<Mesh>(*teapot);
    
    //teapot->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 42.0f));
    teapot->TransformBy(Transform::Scale({0.55f, 0.55f, 0.55f}));
    teapot->TransformBy(Transform::Translate({7.0f, 0.2f, -40.0f}));

    //Mesh test = *teapot;
    //teapot2->TransformBy(Transform::Rotate({0.0f, 1.0f, 0.0f}, 60.0f));
    teapot2->TransformBy(Transform::Scale({0.25f, 0.25f, 0.25f}));
    teapot2->TransformBy(Transform::Translate({7.0f, 0.2f, -20.0f}));

    //auto matte = std::make_shared<Matte>( Color3f{0} );
    //teapot->ApplyMaterial(matte);

    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<std::unique_ptr<Light>> lights;
    meshes.push_back(std::move(teapot));
    meshes.push_back(std::move(teapot2));

    auto scene = std::make_unique<Scene>(std::move(meshes), std::move(lights));
    scene->SetBackgroundColor(Color3f{0.0f});

    auto camera = std::make_unique<Camera>(Point3f(0.0f, 1.0f, 0.0f), Point3f(0.0f,.0f,-8.0f),
                             Vec3f(0.0f,1.0f,0.0f), 60.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
    auto window = std::make_unique<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    auto buffer = std::make_shared<ImageBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    Renderer renderer{ std::move(camera), std::move(scene), buffer };
    std::thread RenderThread{ &Renderer::Render, std::ref(renderer), 20 };

    window->InitializeWindow(*buffer);
    window->CheckForInput(*buffer);

    RenderThread.join();

    return 0;
}