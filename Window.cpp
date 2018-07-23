#include "Window.h"
#include <iostream>

Window::Window()
{
}

Window::~Window()
{
    // deallocate surface
    SDL_FreeSurface( m_screenSurface );

    // destroy window
    SDL_DestroyWindow( m_windowHandle );

    // terminate SDL
    SDL_Quit();
}

bool Window::initializeWindow(const int screenHeight, const int screenWidth)
{
    // initalize window
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError();
        return false;
    }

    // get handle
    m_windowHandle = SDL_CreateWindow( "Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN );

    if (m_windowHandle == NULL)
    {
        std::cout << "SDL window could not be created. Error: " << SDL_GetError();
        return false;
    }

    // get surface of window 
    m_screenSurface = SDL_GetWindowSurface( m_windowHandle );

    // draw, update, delay
    SDL_FillRect( m_screenSurface, NULL, SDL_MapRGB( m_screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    SDL_UpdateWindowSurface( m_windowHandle );
    SDL_Delay( 2000 );

    return true;
}

