#pragma once
#include "../imaging/image_buffer.hpp"
#include <SDL2/SDL.h>


namespace tr {

    class Window {
    public:
        Window(int screen_width, int screen_height);
        ~Window();
        void handle_input(ImageBuffer& pixelBuffer);
        void update_texture(ImageBuffer& buffer);
    
    private:
        SDL_Window *m_windowHandle;
        SDL_Surface *m_screenSurface;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
        SDL_Event m_eventHandler;
        int const m_screen_height;
        int const m_screen_width;
    };

}
