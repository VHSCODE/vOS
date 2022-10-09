#include <stdio.h>

#ifdef GUI
#include <SDL2/SDL.h>
#endif

#include "pthread.h"
#include "machine.h"




int main()
{
	#ifdef GUI
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Error while initializing SDL : %s\n",SDL_GetError());
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("vOS: Kernel Simulator", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_OPENGL);
	if(!window){
		printf("Error while creating window: %s\n", SDL_GetError());
		return -1;
	}

	#endif

	init_machine(1); //init our global machine and threads

	while (g_machine.is_running)
	{
		//Draw thread ?
	}

	deinit_machine();
#ifdef GUI
	SDL_DestroyWindow(window);
	SDL_Quit();
#endif
	return 0;
}
