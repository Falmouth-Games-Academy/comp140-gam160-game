#include "stdafx.h"
#include "Game.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>


using std::cout;
using std::endl;

Player* player;
Enemy* enemy;
Enemy* enemy2;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;


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
	enemy = new Enemy("Assets/Enemy.png", 500, -64);
	enemy2 = new Enemy("Assets/Enemy3.png", 864, 200);
	

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
	




	cnt++;

	player->Update();
	enemy->MoveDown();
	enemy2->MoveLeft();

	cout << cnt << endl;
}


void Game::render()
{
	// set background color
	SDL_SetRenderDrawColor(renderer, 0, 204, 0, 255);

	// clear previous frame
	SDL_RenderClear(renderer);

	
	enemy->Render();
	enemy2->Render();

	player->Render();



	/*
	// draw to the screen here!
	SDL_Rect player1;
	player1.x = 50;
	player1.y = serialInterface->getPot1();
	player1.w = 30;
	player1.h = 80;

	SDL_SetRenderDrawColor(mainRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(mainRenderer, &player1);

	SDL_Rect player2;
	player2.x = 590;
	player2.y = serialInterface->getPot2();
	player2.w = 30;
	player2.h = 80;

	SDL_SetRenderDrawColor(mainRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(mainRenderer, &player2);
	*/

	// render new frame
	SDL_RenderPresent(renderer);
}


/*
* clean - Clean up SDL and close the port
*
*/
void Game::clean()
{
	serialInterface->close();
	cout << "Cleaning SDL \n";
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}