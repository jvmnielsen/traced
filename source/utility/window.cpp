#include "window.hpp"
#include <iostream>

Window::Window(const int screen_width, const int screen_height)
    : m_screen_width(screen_width),
      m_screen_height(screen_height)
{
    bool init_successful = SDL_Init(SDL_INIT_VIDEO) >= 0;

    if (init_successful)
    {
        SDL_CreateWindowAndRenderer(m_screen_width, m_screen_height, SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC,
                                    &m_windowHandle, &m_renderer);

        SDL_SetWindowTitle(m_windowHandle, "Traced");

        m_screenSurface = SDL_GetWindowSurface(m_windowHandle);

        m_texture = SDL_CreateTexture(m_renderer,
                                      SDL_PIXELFORMAT_RGBA32,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      screen_width,
                                      screen_height);
    }
    else {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError();
    }
}

/*
auto Window::initialize_window(ImageBuffer& buffer) -> bool
{
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError();
        return false;
    }

    SDL_CreateWindowAndRenderer(m_screen_width, m_screen_height, SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC, &m_windowHandle, &m_renderer);

    SDL_SetWindowTitle(m_windowHandle, "Traced");


    m_texture = SDL_CreateTexture(
            m_renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            buffer.GetWidth(),
            buffer.GetHeight());

    return true;
} */

Window::~Window()
{
    SDL_FreeSurface( m_screenSurface );

    SDL_DestroyTexture( m_texture );

    SDL_DestroyWindow( m_windowHandle );

    SDL_DestroyRenderer( m_renderer );

    SDL_Quit();
}

auto Window::update_texture(ImageBuffer& buffer) -> void
{
    //int pitch = buffer.GetWidth() * sizeof(uint32_t);
    //SDL_LockTexture(m_texture, NULL, (void**)&buffer.m_buffer[0], &pitch);
    SDL_UpdateTexture(m_texture, NULL, &buffer.m_buffer[0], buffer.get_width() * sizeof(uint32_t));
    //m_pixels = &buffer.m_buffer[0];
    //SDL_UnlockTexture(m_texture);

    SDL_RenderClear( m_renderer ); // might not be necessary
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

auto Window::handle_input(ImageBuffer& pixelBuffer) -> void
{
    bool running = true;

    //SDL_SetWindowPosition(m_windowHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);


    Uint32 startTime = 0;
    Uint32 endTime = 0;
    Uint32 delta = 0;
    short timePerFrame = 32; // miliseconds


    //While application is running 
    while (running)
    {

        if (!startTime) {
            // get the time in ms passed from the moment the program started
            startTime = SDL_GetTicks();
        }
        else {
            delta = endTime - startTime; // how many ms for a frame
        }

        // if less than 16ms, delay 
        if (delta < timePerFrame) {
            SDL_Delay(timePerFrame - delta);
        }

        update_texture(pixelBuffer);

        //Handle events on queue
        if (SDL_PollEvent(&m_eventHandler))
        {
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
    SDL_QueryTexture(m_texture, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(m_renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, "out.bmp");
    SDL_FreeSurface(surface);


    //SDL_SavePNG(m_screenSurface, "out.bmp");
}



