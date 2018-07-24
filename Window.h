#pragma once
#include <SDL.h>
#include <memory>
#include <vector>
#include "PixelBuffer.h"

class Window
{
public:
	Window( const int screenHeight, const int screenWidth );
	
	~Window();

	bool initializeWindow();

    void renderPixelBuffer( PixelBuffer &pixelBuffer );
    void renderPixelBuffer( char* pixels );
private:
	SDL_Window *m_windowHandle;
	SDL_Surface *m_screenSurface;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;

    int m_screenHeight;
    int m_screenWidth;
};

