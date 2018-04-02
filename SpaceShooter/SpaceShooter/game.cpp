#include <SDL.h>
#include <algorithm>
#include <iostream>

#include "game.h"
#include "graphics.h"
#include "input.h"

//Game class
//Holds all the information for the main game loop, it's essentially a game manager

namespace
{
	const int FPS = 60;
	const int MAX_FRAME_TIME = 6 * 1000 / FPS;
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
	Input input;
	SDL_Event event;

	player = Player(graphics, globals::SCREEN_WIDTH / 2 - 64, 600);

	int LAST_UPDATE_TIME = SDL_GetTicks();
	//Start of the game loop
	while (running == true) {
		input.beginNewFrame();

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.repeat == 0)
				{
					input.KeyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				input.KeyUpEvent(event);
			}
			else if (event.type == SDL_QUIT)
			{
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
		{
			return;
		}

		else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true || input.isKeyHeld(SDL_SCANCODE_A) == true)
		{
			player.moveLeft();
			std::cout << player.x << std::endl;
		}

		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true || input.isKeyHeld(SDL_SCANCODE_D) == true)
		{
			player.moveRight();
			std::cout << player.x << std::endl;
		}

		else if (input.isKeyHeld(SDL_SCANCODE_UP) == true || input.isKeyHeld(SDL_SCANCODE_W) == true)
		{
			player.moveUp();
			std::cout << player.y << std::endl;
		}

		else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true || input.isKeyHeld(SDL_SCANCODE_S) == true)
		{
			player.moveDown();
			std::cout << player.y << std::endl;
		}

		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT) && !input.isKeyHeld(SDL_SCANCODE_A) && !input.isKeyHeld(SDL_SCANCODE_D) && !input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_W) && !input.isKeyHeld(SDL_SCANCODE_DOWN) && !input.isKeyHeld(SDL_SCANCODE_S))
		{
			player.stopMoving();
		}
		
		const int CURENT_TIME_MS = SDL_GetTicks();
		int ELAPSE_TIME_MS = CURENT_TIME_MS - LAST_UPDATE_TIME;
		update(std::min(ELAPSE_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURENT_TIME_MS;

		drawGraphics(graphics);
	}

}

void Game::drawGraphics(Graphics &graphics)
{
	graphics.clear();

	player.draw(graphics);

	graphics.flip();
}

void Game::update(float elapsedTime)
{
	player.update(elapsedTime);
	//level.update(elapsedTime);
}