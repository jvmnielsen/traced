#pragma once
#include <SDL2/SDL.h>
#include "../imaging/image_buffer.hpp"

namespace tr {

    class Window
    {
    public:
        Window(int screen_width, int screen_height);
        ~Window();

        auto initialize_window(ImageBuffer& buffer) -> bool;

        void handle_input(ImageBuffer& pixelBuffer);
        void update_texture(ImageBuffer& buffer);
    
    private:
        SDL_Window *m_windowHandle;
        SDL_Surface *m_screenSurface;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;

        //Event handler
        SDL_Event m_eventHandler;

        //void* m_pixels; // buffer that holds pixels for screen

        const int m_screen_height;
        const int m_screen_width;
    };

}
