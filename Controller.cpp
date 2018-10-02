#include "Controller.h"
#include <thread>

void Controller::run()
{
    // window thread
    m_window.initializeWindow();

    //m_scene.render(m_buffer);

    std::thread render_thread{ &Scene::render, m_scene, std::ref( m_buffer ) };

    //std::thread window_thread{ &Window::render_buffer, m_window, std::ref( m_buffer ) };
   // m_window.render_buffer( m_buffer ); // only main thread is allowed to interact with window

    // render thread
   
    //m_scene.render( m_buffer );

    m_window.check_for_input( m_buffer );

    //window_thread.join();
    render_thread.join();
}
