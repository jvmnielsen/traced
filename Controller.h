#pragma once
#include "Scene.h"
#include "Window.h"
#include "ImageBuffer.h"

class Controller
{
public:
    Controller(Scene& scene) 
        : m_scene(scene) 
        , m_window(scene.m_screen_width, scene.m_screen_height)
        , m_buffer(4, scene.m_screen_width, scene.m_screen_height)
    {
    }

    

    void run();

private:
    Scene m_scene;
    Window m_window;
    ImageBuffer m_buffer;
    int m_screen_channels;
};

