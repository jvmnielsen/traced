#include "ThreadManager.h"
#include <thread>

/*
void ThreadManager::Run(const Scene& scene)
{

    //std::thread render_thread{ &Scene::Render, scene, *m_buffer };

    // window thread
    m_window->InitializeWindow(*m_buffer);

    //m_scene->Render(*m_buffer);



    //std::thread window_thread{ &Window::render_buffer, m_window, std::ref( m_buffer ) };
   // m_window.render_buffer( m_buffer ); // only main thread is allowed to interact with window

    // render thread
   
    //m_scene.render( m_buffer );

    m_window->CheckForInput(*m_buffer);

    //window_thread.join();
    //render_thread.join();
} */
