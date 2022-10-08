#include <stdio.h>
//#include <SDL2/SDL.h>


#include "pthread.h"
#include "machine.h"




int main()
{

	/*
	//TODO check for errors
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window * window = SDL_CreateWindow("vOS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED |SDL_RENDERER_PRESENTVSYNC);
		*/

	init_machine(1); //init our global machine and threads

	while (g_machine.is_running)
	{
		//Draw thread ?
	}

	deinit_machine();
}
