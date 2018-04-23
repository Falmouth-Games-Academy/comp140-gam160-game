#include "stdafx.h"
#include "Game.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>
#include <time.h>       /* time */


using std::cout;
using std::endl;

Player* player;
Enemy* enemy;
Enemy* enemy2;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int numberofenemies = 5;


/*
* Constructor - not used in this case
*
*/
Game::Game()
{
	serialInterface = new SerialInterface();

}

/*
Decontructor - also not used in this case
*/
Game::~Game()
{
}

/*
* init - used to initialise and setup SDL
* Return true if everything is successful
*/
bool Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	srand(time(NULL));
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "SDL init success \n";

		// Create a window
		mainWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		// if window succesful..
		if (mainWindow != 0)
		{
			cout << "Window creation success \n";

			// create renderer
			renderer = SDL_CreateRenderer(mainWindow, -1, 0);

			// if renderer successful...
			if (renderer != 0)
			{
				cout << "Renderer creation success \n";
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			else
			{
				cout << "renderer failed \n";
				return false;
			}
		}
		else
		{
			cout << "window failed \n";
			return false;
		}


	}
	else
	{
		cout << "SDL fail \n";
		return false;
	}

	isRunning = true;
	cout << "SDL init success \n";

	//instantiate serial here

	player = new Player("Assets/Crosshair.png", 800 / 2, 640 / 2);
	player->serialInterface = serialInterface;

	for (int i = 0; i < numberofenemies; i++)
	{
		enemylist.push_back(new Enemy("Assets/Enemy.png", rand() % 736 + 1, -64));
	}

	return true;
}


/*
* handleEvents - Poll Events and uses switch case to process specific events
*
*/
void Game::handleEvents()
{
	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		default:
			break;
		}

	}
}


/*
* update - Process all variables !in charge of anything to do with rendering
*
*/
void Game::update()
{
	//update player positions
	player->Update();

	//if player fires the cannon while intersecting with an enemy, reset that enemy's
	//position to the top of the screen and increase player score
	if (player->hasFired())
	{
		for (Enemy* currentEnemy : enemylist)
		{
			if (currentEnemy->isPointInside(player->xpos + 32, player->ypos + 32))
			{
				currentEnemy->ypos = -64;
				currentEnemy->xpos = rand() % 736 + 1;
				score++;
				cout << "Score: " << score << endl;
			}
		}
	}

	// Using the console to show the scoring and lives left.
	for (Enemy* currentEnemy : enemylist)
	{
		//change the enemies speed depending on how high the players score is
		if (score < 5)
		{
			currentEnemy->MoveDown();
		}

		if ((score >= 5) && (score < 11))
		{
			currentEnemy->MoveDown2();
		}

		if (score >= 11)
		{
			currentEnemy->MoveDown2();
		}

		//at end of game stop all enemies
		if (lives == 0)
		{
			currentEnemy->StopMoving();
		}

		//respawn the enemy at the top of the screen if the player misses it and
		//lower the players number of lives by one 
		if (currentEnemy->ypos > 704)
		{
			currentEnemy->ypos = -64;
			currentEnemy->xpos = rand() % 736 + 1;
			lives--;
			cout << "Lives left: " << lives - 1 << endl;

			//end of game, show final score
			if (lives == 0)
			{
				cout << "Game Over!" << endl;
				cout << "Final Score: " << score << endl;
				score = 0;
			}
		}
	}
}


void Game::render()
{
	// set background color
	SDL_SetRenderDrawColor(renderer, 0, 204, 0, 255);

	// clear previous frame
	SDL_RenderClear(renderer);

	//render enemies from vector
	for (Enemy* currentEnemy : enemylist)
	{
		currentEnemy->Render();
	}

	//render player
	player->Render();

	// render new frame
	SDL_RenderPresent(renderer);
}


/*
* clean - Clean up SDL and close the port
*
*/
void Game::clean()
{
	// delete player and enemies
	delete player;
	for (auto iter = enemylist.begin(); iter != enemylist.end(); )
	{
		if ((*iter))
		{
			delete (*iter);
			iter = enemylist.erase(iter);
		}

		else
		{
			iter++;
		}
	}
	enemylist.clear();
	serialInterface->close();
	cout << "Cleaning SDL \n";
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}