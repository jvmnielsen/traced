#include "window.hpp"
#include <iostream>
#include <ratio>
#include <chrono>
#include <algorithm>

Window::Window( const uint32_t screenWidth,
                const uint32_t screenHeight )
    : m_screenWidth( screenWidth )
    , m_screenHeight( screenHeight )
{
    m_screenSurface = NULL;
    m_texture = NULL;
    m_windowHandle = NULL;
}

Window::~Window()
{
    SDL_FreeSurface( m_screenSurface );

    SDL_DestroyTexture( m_texture );

    SDL_DestroyWindow( m_windowHandle );

    SDL_DestroyRenderer( m_renderer );

    SDL_Quit();
}

bool Window::InitializeWindow(ImageBuffer& buffer)
{
    // initalize window
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError();
        return false;
    }

    // get handle
    m_windowHandle = SDL_CreateWindow(
            "Traced",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_screenWidth,
            m_screenHeight,
            0);

    if (m_windowHandle == NULL)
    {
        std::cout << "SDL window could not be created. Error: " << SDL_GetError();
        return false;
    }

    // create renderer here for now
    m_renderer = SDL_CreateRenderer(m_windowHandle, -1, SDL_RENDERER_SOFTWARE);

    // get surface of window 
    m_screenSurface = SDL_GetWindowSurface( m_windowHandle );

    if (m_screenSurface == NULL)
    {
        std::cout << "SDL surface could not be created. Error: " << SDL_GetError();
        return false;
    }

    m_texture = SDL_CreateTexture(
            m_renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            buffer.GetWidth(),
            buffer.GetHeight());

    m_pixels = &buffer.m_buffer[0]; //&buffer.m_pixel_data[0];

    SDL_SetWindowPosition(m_windowHandle, 0, 0);


    return true;
}

void Window::UpdateTexture(ImageBuffer &buffer)
{
    SDL_UpdateTexture(m_texture, NULL, m_pixels, buffer.GetWidth() * sizeof(uint32_t));

    //SDL_RenderClear( m_renderer ); // might not be necessary
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

void Window::CheckForInput(ImageBuffer &pixelBuffer)
{
    bool running = true;

    SDL_SetWindowPosition(m_windowHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    //While application is running 
    while (running)
    {
        UpdateTexture(pixelBuffer);

        //Handle events on queue
        if (SDL_PollEvent(&m_eventHandler))
        {
            //User requests quit
            if (m_eventHandler.type == SDL_QUIT) {
                //SDL_SaveBMP( m_screenSurface, "test.bmp" );
                running = false;
            }
        }
    } 
}



