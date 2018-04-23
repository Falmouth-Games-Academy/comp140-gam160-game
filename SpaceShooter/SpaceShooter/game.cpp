#include <SDL.h>
#include <algorithm>
#include <iostream>

#include "game.h"
#include "graphics.h"
#include "input.h"
#include "controls.h"

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
	srand(SDL_GetPerformanceCounter());
	gameLoop();
}

Game::~Game()
{
	for (auto iter = enemyList.begin(); iter != enemyList.end(); )
	{
		if ((*iter))
		{
			delete (*iter);
			(*iter) = nullptr;
			iter = enemyList.erase(iter);
		}
		else
		{
			iter++;
		}

	}
}

void Game::gameLoop()
{
	Graphics graphics;
	Input input;
	Controls controls;
	SDL_Event event;
	

	level = Level(graphics, 0,0);
	//projectile = Projectile(graphics, 100, 400);
	player = Player(graphics, globals::SCREEN_WIDTH / 2 - 64, 600);
	
	player.initBullets(graphics);
	
	for (int i = 0; i < 5; i++)
	{
		Enemy* newEnemy = new Enemy(graphics, 0, 0);
		enemyList.push_back(newEnemy);
		newEnemy->respawn();
	}
	
	//enemy = Enemy(graphics, 0, 0);
	//enemy.respawn();

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
				running = false;
			}
		}

		// exit the game if escape is pressed
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
		{
			running = false;
		}

		// what allows the player to control the spaceship
		controls.playerControls(player, input, graphics);
		
		// frame rate handler
		const int CURENT_TIME_MS = SDL_GetTicks();
		int ELAPSE_TIME_MS = CURENT_TIME_MS - LAST_UPDATE_TIME;
		update(std::min(ELAPSE_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURENT_TIME_MS;

		// update the graphics ons screen
		drawGraphics(graphics);
	}

}

void Game::drawGraphics(Graphics &graphics)
{
	graphics.clear();

	level.draw(graphics);
	player.draw(graphics);
	//enemy.draw(graphics);

	for (Enemy* enemy : enemyList)
	{
		enemy->draw(graphics);
	}

	graphics.flip();
}

void Game::update(float elapsedTime)
{
	level.update(elapsedTime);
	player.levelCollisions(player.playerBoxCollider, level.levelBoxCollider);
	player.update(elapsedTime);
	//enemy.update(elapsedTime);

	for (Enemy* enemy : enemyList)
	{
		enemy->update(elapsedTime);
	}

	for (Projectile* bullet : player.getBullets())
	{

		if (bullet->isActive)
		{
			for (Enemy* enemy : enemyList)
			{
				if (bullet->checkCollision(enemy))
				{
					bullet->isActive = false;
					enemy->respawn();
				}
			}
		}

	}
	
	for (Enemy* enemy : enemyList)
	{
		if (player.checkCollision(enemy))
		{
			enemy->respawn();
		}
	}

	
}