#pragma once
#include "Scene.h"
#include "Window.h"
#include "Imaging.h"

class ThreadManager
{
public:
    ThreadManager(
            //std::unique_ptr<Scene> scene,
            std::unique_ptr<Window> window,
            std::unique_ptr<ImageBuffer> buffer)
        //: m_scene(std::move(scene))
        : m_window(std::move(window))
        , m_buffer(std::move(buffer))
    {
    }

    //~ThreadManager() = delete;

    void Run(const Scene& scene);

private:
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<ImageBuffer> m_buffer;
};

