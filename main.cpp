#include "Window.h"
#include <iostream>

// arguments necessary for SDL to be multi-platform
int main(int argc, char * argv[])
{
    Window window = Window();
    window.initializeWindow(400, 400);

	return 0;
}