#pragma once
#include <SDL2/SDL.h>
#include "../Imaging/ImageBuffer.hpp"

class Window
{
public:
	Window(const uint32_t screenHeight, const uint32_t screenWidth);
	
    Window();
	~Window();

	bool InitializeWindow(ImageBuffer &buffer);

    void CheckForInput(ImageBuffer &pixelBuffer);
    void UpdateTexture(ImageBuffer &buffer);
   
private:
	SDL_Window *m_windowHandle;
	SDL_Surface *m_screenSurface;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;

    //Event handler
    SDL_Event m_eventHandler;

    void* m_pixels; // buffer that holds pixels for screen

    uint32_t m_screenHeight;
    uint32_t m_screenWidth;
};

