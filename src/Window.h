#pragma once
#include <memory>
#include <vector>
#include "Imaging.h"
#include <SDL.h>

class Window
{
public:
	Window( const int screenHeight, const int screenWidth );
	
    Window();
	~Window();

	bool initializeWindow(ImageBuffer& buffer);

    void check_for_input( ImageBuffer &pixelBuffer );
    void update_texture( ImageBuffer& buffer );
   
private:
	SDL_Window *m_windowHandle;
	SDL_Surface *m_screenSurface;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;

    void* m_pixels; // buffer that holds pixels for screen

    int m_screenHeight;
    int m_screenWidth;
};

