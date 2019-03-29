#include "window.hpp"
#include <iostream>

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

    //SDL_Window* window;
    //SDL_Renderer* renderer;

    SDL_CreateWindowAndRenderer(m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC, &m_windowHandle, &m_renderer);

    SDL_SetWindowTitle(m_windowHandle, "Traced");

    /*
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
    //m_renderer = SDL_CreateRenderer(m_windowHandle, -1, SDL_RENDERER_SOFTWARE);

    

    if (m_screenSurface == NULL)
    {
        std::cout << "SDL surface could not be created. Error: " << SDL_GetError();
        return false;
    } */

    // get surface of window 
    m_screenSurface = SDL_GetWindowSurface(m_windowHandle);

    m_texture = SDL_CreateTexture(
            m_renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            buffer.GetWidth(),
            buffer.GetHeight());

    //m_pixels = &buffer.m_buffer[0]; //&buffer.m_pixel_data[0];

    
    return true;
}

void Window::UpdateTexture(ImageBuffer& buffer)
{
    //int pitch = buffer.GetWidth() * sizeof(uint32_t);
    //SDL_LockTexture(m_texture, NULL, (void**)&buffer.m_buffer[0], &pitch);
    SDL_UpdateTexture(m_texture, NULL, &buffer.m_buffer[0], buffer.GetWidth() * sizeof(uint32_t));
    //m_pixels = &buffer.m_buffer[0];
    //SDL_UnlockTexture(m_texture);

    SDL_RenderClear( m_renderer ); // might not be necessary
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

void Window::CheckForInput(ImageBuffer &pixelBuffer)
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



