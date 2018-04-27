
// Using SDL and standard IO
// Parts of Arduino code take from playground.arduino.cc and altered to work with the specifics of my controller.
#include <SDL.h>
#include "Game.h"
#include "stdafx.h"
#include <stdio.h>

Game* game = 0;

int screenWidth = 800;
int screenHeight = 640;


int main(int argc, char* argv[])
{

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	// new game instance
	game = new Game();

	// initialise the game
	game->init("Mortar Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, false);

	// create a game loop
	while (game->running())
	{

		frameStart = SDL_GetTicks();

		// choose a framerate - in this case we are using 1000%40 which means 
		// 25 frames per second (quite slow)
		//int increment = SDL_GetTicks() % 40;

		// if modulo returns 0 trigger one frame in our game
		//if (increment == 0) {

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}

	}

	// when the game ends clean SDL and Serial
	game->clean();

	return 0;
}