#include "utility/utility.hpp"
#include "utility/parser.hpp"
#include "core/scene.hpp"
#include "imaging/camera.hpp"
#include "utility/window.hpp"
#include "imaging/image_buffer.hpp"
#include "core/renderer.hpp"
#include "material/material.hpp"

#include <graphics-math.hpp>

#include <thread>

// arguments necessary for SDL to be multi-platform
int main(int argc, char * argv[]) {
    
    auto constexpr screen_width = 720;
    auto constexpr screen_height = 480;
    auto constexpr rpp = 2048;

    auto matte = std::make_shared<tr::Matte>(gm::Color3f{0.5, 0.5, 0.5});
    auto dark = std::make_shared<tr::Matte>(gm::Color3f{0.02, 0.02, 0.02});
    auto green = std::make_shared<tr::Matte>(gm::Color3f{0.1, 0.3, 0.1});
    auto red = std::make_shared<tr::Matte>(gm::Color3f{0.4, 0.1, 0.1});
    auto blue = std::make_shared<tr::Matte>(gm::Color3f{0.1, 0.1, 0.4});
    auto light = std::make_shared<tr::Emissive>();

#ifdef _MSC_VER
    auto const cube = "assets/cube.obj";
    auto const plane = "assets/plane.obj";
    auto const bunny = "assets/bunny.obj";
    auto const sphere = "assets/sphere2.obj";
#else 
    auto const cube = "../../assets/cube.obj";
    auto const plane = "../../assets/plane.obj";
    auto const bunny = "../../assets/bunny.obj";
    auto const sphere = "../../assets/sphere2.obj";
#endif

    auto const bunny_transform = gm::Transform().translate({-3, 0.5, 4.5}).rotate({0, 1, 0}, 35).scale(gm::Vec3f{7.5});
    auto const ball1_transform = gm::Transform().translate({8, 6.7, 1.5}).scale(gm::Vec3f{6.2});
    auto const ball2_transform = gm::Transform().translate({6, 3.7, 10.5}).scale(gm::Vec3f{3.2});
    auto const floor_transform = gm::Transform().scale(gm::Vec3f(200.0));
    auto const light_transform = gm::Transform().translate({0, 22., 2.0}).rotate({0.0f, .0f, 5.}, -170.0).scale(gm::Vec3f{14.0});
    auto const right_wall_transform = gm::Transform().translate({20., 0.0, 0.0}).rotate({0.0, 0.0, 1.0}, 90.0).scale(gm::Vec3f{20.0});
    auto const left_wall_transform = gm::Transform().translate({-20.0, 0.0, 0.0}).rotate({0., 0.0, 1.0}, -90.0).scale(gm::Vec3f{20.0});
    auto const back_wall_transform = gm::Transform().translate({0.0, 0., -20.0}).rotate({1.0, 0.0, 0.0}, 90.0).scale(gm::Vec3f{20.});

    tr::Parser parser;
    auto bunny_model = parser.construct_mesh_from_file(bunny, green, bunny_transform);
    auto ball1 = parser.construct_mesh_from_file(sphere, matte, ball1_transform);
    auto ball2 = parser.construct_mesh_from_file(sphere, matte, ball2_transform);
    auto light_source = parser.construct_mesh_from_file(plane, light, light_transform);
    auto floor = parser.construct_mesh_from_file(plane, matte, floor_transform);
    auto right_wall = parser.construct_mesh_from_file(plane, blue, right_wall_transform);
    auto left_wall = parser.construct_mesh_from_file(plane, red, left_wall_transform);
    auto back_wall = parser.construct_mesh_from_file(plane, matte, back_wall_transform);


    std::vector<std::unique_ptr<tr::Mesh>> meshes;
    meshes.push_back(std::move(bunny_model));
    meshes.push_back(std::move(ball1));
    meshes.push_back(std::move(ball2));
    meshes.push_back(std::move(floor));
    meshes.push_back(std::move(right_wall));
    meshes.push_back(std::move(left_wall));
    meshes.push_back(std::move(back_wall));

    std::vector<std::unique_ptr<tr::Mesh>> lights;
    lights.push_back(std::move(light_source));

    auto scene = std::make_unique<tr::Scene>(std::move(meshes), std::move(lights));

    auto const look_from = gm::Point3f(0.0, 16.0, 24.0);
    auto const look_at = gm::Point3f(0.0, 0.0, -1.5);
    auto const dist_to_focus = (look_from - look_at).length() - 15;
    auto const aperture = 0;
    auto const aspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);
    auto camera = std::make_unique<tr::Camera>(look_from, look_at, 55.0f, aspect, aperture, dist_to_focus);   

    auto buffer = std::make_shared<tr::ImageBuffer>(screen_width, screen_height);
    
    tr::Renderer renderer{ std::move(camera), std::move(scene), buffer };
    std::thread render_thread{ &tr::Renderer::render, std::ref(renderer), rpp };
    std::cout << "Render-thread started\n";

    auto window = std::make_unique<tr::Window>(screen_width, screen_height);
    window->handle_input(*buffer);

    renderer.shutdown();
    render_thread.join();

    return 0;
}