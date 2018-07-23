#pragma once
#include <SDL.h>
#include <memory>

class Window
{
public:
	Window();
	
	~Window();

	bool initializeWindow(const int screenHeight, const int screenWidth);


private:
	SDL_Window *m_windowHandle;
	SDL_Surface *m_screenSurface;
};

