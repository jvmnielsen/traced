#include "window.hpp"
#include <iostream>

using namespace tr;

Window::Window(const int screen_width, const int screen_height)
    : m_screen_width(screen_width),
      m_screen_height(screen_height) {

    bool init_successful = SDL_Init(SDL_INIT_VIDEO) >= 0;

    if (init_successful) {
        SDL_CreateWindowAndRenderer(
            m_screen_width,
            m_screen_height,
            SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC,
            &m_windowHandle, &m_renderer
        );

        SDL_SetWindowTitle(m_windowHandle, "Traced");

        m_screenSurface = SDL_GetWindowSurface(m_windowHandle);

        m_texture = SDL_CreateTexture(
            m_renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            screen_width,
            screen_height
        );
    } else {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError();
    }
}

Window::~Window() {
    SDL_FreeSurface(m_screenSurface);
    SDL_DestroyTexture(m_texture);
    SDL_DestroyWindow(m_windowHandle);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
}

auto Window::update_texture(ImageBuffer& buffer) -> void {
    SDL_UpdateTexture(
        m_texture, nullptr,
        &buffer.m_buffer[0],
        buffer.width() * sizeof(uint32_t)
    );
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

auto Window::handle_input(ImageBuffer& pixelBuffer) -> void {
    bool running = true;
    Uint32 startTime = 0;
    Uint32 endTime = 0;
    Uint32 delta = 0;
    Uint32 timePerFrame = 32; // miliseconds

    //While application is running 
    while (running) {
        if (!startTime) {
            // get the time in ms passed from the moment the program started
            startTime = SDL_GetTicks();
        } else {
            delta = endTime - startTime; // how many ms for a frame
        }
        // if less than 16ms, delay 
        if (delta < timePerFrame) {
            SDL_Delay(timePerFrame - delta);
        }
        update_texture(pixelBuffer);
        //Handle events on queue
        if (SDL_PollEvent(&m_eventHandler)) {
            //User requests quit
            if (m_eventHandler.type == SDL_QUIT) {
                //SDL_SaveBMP( m_screenSurface, "test.bmp" );
                running = false;
            }
        }
        startTime = endTime;
        endTime = SDL_GetTicks();
    }

    int width, height;
    SDL_QueryTexture(m_texture, nullptr, nullptr, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(m_renderer, nullptr, surface->format->format, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, "out.bmp");
    SDL_FreeSurface(surface);
}



