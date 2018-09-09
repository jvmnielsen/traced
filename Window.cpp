#include "Window.h"
#include <iostream>

Window::Window( const int screenWidth,
                const int screenHeight )
    : m_screenWidth( screenWidth )
    , m_screenHeight( screenHeight )
{
    m_screenSurface = NULL;
    m_texture = NULL;
    m_windowHandle = NULL;
}

Window::~Window()
{
    // deallocate surface
    SDL_FreeSurface( m_screenSurface );

    SDL_DestroyTexture( m_texture );

    // destroy window
    SDL_DestroyWindow( m_windowHandle );

    // terminate SDL
    SDL_Quit();
}

bool Window::initializeWindow()
{
    // initalize window
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError();
        return false;
    }

    // get handle
    m_windowHandle = SDL_CreateWindow( "Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN );

    if (m_windowHandle == NULL)
    {
        std::cout << "SDL window could not be created. Error: " << SDL_GetError();
        return false;
    }

    // create renderer here for now
    m_renderer = SDL_CreateRenderer( m_windowHandle, -1, 0 );

    
    // get surface of window 
    m_screenSurface = SDL_GetWindowSurface( m_windowHandle );

    if (m_screenSurface == NULL)
    {
        std::cout << "SDL surface could not be created. Error: " << SDL_GetError();
        return false;
    }
    
    return true;
}

void Window::update_texture( PixelBuffer& buffer )
{
    void *data = &buffer.m_pixel_data[0];

    m_screenSurface = SDL_CreateRGBSurfaceWithFormatFrom( data,
                                                          buffer.m_screenWidth,
                                                          buffer.m_screenHeight,
                                                          buffer.m_channels * 8,                         // bits per byte
                                                          buffer.m_screenWidth * buffer.m_channels, // how many pixels per line (depth * width)
                                                          SDL_PIXELFORMAT_RGBA32 );


    //SDL_SaveBMP( m_screenSurface, "test.bmp" );

    m_texture = SDL_CreateTextureFromSurface( m_renderer,
                                              m_screenSurface );

    SDL_RenderClear( m_renderer );
    SDL_RenderCopy( m_renderer, m_texture, NULL, NULL );
    SDL_RenderPresent( m_renderer );
}

void Window::render_buffer( PixelBuffer &pixelBuffer )
{
    bool running = true;

    //Event handler
    SDL_Event eventHandler;

    //While application is running 
    while (running)
    {
        //Handle events on queue
        while (SDL_PollEvent( &eventHandler ) != 0)
        {
       
            //User requests quit
            if (eventHandler.type == SDL_QUIT)
            {
                running = false;
            }

            
        }

        update_texture( pixelBuffer );
    } 
}



