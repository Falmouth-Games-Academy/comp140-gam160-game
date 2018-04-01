#include <SDL.h>
#include <algorithm>

#include "game.h"
#include "graphics.h"

//Game class
//Holds all the information for the main game loop, it's essentially a game manager

namespace
{
	const int FPS = 60;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	gameLoop();
}

Game::~Game()
{

}

void Game::gameLoop()
{
	Graphics graphics;
	SDL_Event event;

	int LAST_UPDATE_TIME = SDL_GetTicks();
	
	while (running == true) {
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_Quit();
				running = false ;
			}
		}
		
		const int CURENT_TIME_MS = SDL_GetTicks();
		int ELAPSE_TIME_MS = CURENT_TIME_MS - LAST_UPDATE_TIME;
		update(std::min(ELAPSE_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURENT_TIME_MS;

		draw(graphics);
	}

}

void Game::draw(Graphics &graphics)
{
	graphics.UpdateGraphics();
	
}

void Game::update(float elapsedTime)
{
	player.update(elapsedTime);
	//level.update(elapsedTime);
}