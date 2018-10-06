#pragma once
#include <memory>
#include <vector>
#include "Imaging.h"
#include <SDL2/SDL.h>

class Window
{
public:
	Window(const int screenHeight, const int screenWidth);
	
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

    int m_screenHeight;
    int m_screenWidth;
};

