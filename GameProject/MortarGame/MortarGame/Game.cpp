#include "stdafx.h"
#include "Game.h"
#include "GraphicsManager.h"
#include "Object.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include <iostream>
#include <time.h>       /* time */


using std::cout;
using std::endl;

Player* player;
Enemy* enemy;
Boss* boss;
Object* explosion;
Object* restart;
Background* background;
Object* redCrosshair;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int numberofenemies = 2;
int numberofbosses = 1;
int bossHealth = 2;


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
	explosion = new Object("Assets/Explosion2.png", player->xpos, player->ypos);
	restart = new Object("Assets/RestartButton.png", 0, 0);
	redCrosshair = new Object("Assets/CrosshairRed.png", player->xpos, player->ypos);

	background = new Background("Assets/Grass.png", 0, 0);

	for (int i = 0; i < numberofenemies; i++)
	{
		enemylist.push_back(new Enemy("Assets/Enemy.png", rand() % 736 + 1, -64));
	}

	for (int i = 0; i < numberofbosses; i++)
	{
		bosslist.push_back(new Boss("Assets/Boss.png", 277, -256));
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

	explosion->Update();
	redCrosshair->Update();

	background->Update();

	restart->Update();

	explosion->xpos = player->xpos;
	explosion->ypos = player->ypos;

	redCrosshair->xpos = player->xpos;
	redCrosshair->ypos = player->ypos;

	//if player fires the cannon while intersecting with an enemy, reset that enemy's
	//position to the top of the screen and increase player score
	if (player->hasFired())
	{
		cout << "Fired!!!" << endl;
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

		for (Boss* currentBoss : bosslist)
		{
			if (currentBoss->isPointInside(player->xpos + 32, player->ypos + 32))
			{
				//reduce health of boss
				if (bossHealth > 0)
				{
					bossHealth--;
				}
				if (bossHealth <= 0)
				{
					bossHealth = 2;
					currentBoss->ypos = -256;
					currentBoss->xpos = 277;
					score = score + 3;
					cout << "Score: " << score << endl;
				}
				
			}
		}

		if (restart->isPointInside(player->xpos + 32, player->ypos + 32))
		{
			gameOver = true;

		}
	}

	
	if (player->xpos < -64)
	{
		player->xpos = 800;
	}

	if (player->xpos > 800)
	{
		player->xpos = -64;
	}

	if (player->ypos < -64)
	{
		player->ypos = 640;
	}

	if (player->ypos > 640)
	{
		player->ypos = -64;
	}

	if (gameOver == true)
	{
		lives = 4;
		score = 0;
		cout << "New Game Started" << endl;
		cout << "Score: " << score << endl;
		if (enemylist.size() > numberofenemies)
		{
			enemylist.erase(enemylist.begin(),enemylist.begin() + extraEnemies);
		}

		if (bosslist.size() > numberofbosses)
		{
			bosslist.erase(bosslist.begin(), bosslist.begin() + extraBosses);
		}

		enemySpeed = 0.5;
		bossSpeed = 1.0;
		extraEnemies = 0;
		extraBosses = 0;
			
		gameOver = false;

		for (Enemy* currentEnemy : enemylist)
		{
			currentEnemy->ypos = -64;
			currentEnemy->xpos = rand() % 736 + 1;
		}

		for (Boss* currentBoss : bosslist)
		{
			currentBoss->ypos = -64;
			currentBoss->xpos = rand() % 736 + 1;
		}
	}

	//change the enemies speed depending on how high the players score is
	if (score < 3)
	{
		enemySpeed = 0.5;
	}

	if ((score >= 3) && (score < 6))
	{
		enemySpeed = 1.0;
		if (enemylist.size() <= 2)
		{
			enemylist.push_back(new Enemy("Assets/Enemy.png", rand() % 736 + 1, -64));
			extraEnemies++;
		}
	}

	if ((score >= 6) && (score < 9))
	{
		enemySpeed = 1.5;
		bossSpeed = 2.0;
		if (enemylist.size() <= 3)
		{
			enemylist.push_back(new Enemy("Assets/Enemy.png", rand() % 736 + 1, -64));
			bosslist.push_back(new Boss("Assets/Boss.png", 277, -256));
			extraEnemies++;
			extraBosses++;
		}
	}

	if ((score >= 9) && (score < 12))
	{
		enemySpeed = 2.0;
	}

	if ((score >= 12) && (score < 15))
	{
		enemySpeed = 2.5;
		if (enemylist.size() <= 4)
		{
			enemylist.push_back(new Enemy("Assets/Enemy.png", rand() % 736 + 1, -64));
			extraEnemies++;
		}
	}

	if ((score >= 15) && (score < 18))
	{
		enemySpeed = 3.0;
	}

	if (score >= 18)
	{
		enemySpeed = 3.5;
		if (enemylist.size() <= 5)
		{
			enemylist.push_back(new Enemy("Assets/Enemy.png", rand() % 736 + 1, -64));
			extraEnemies++;
		}
	}


	// Using the console to show the scoring and lives left.
	for (Enemy* currentEnemy : enemylist)
	{
		currentEnemy->MoveDown(enemySpeed);
		
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
				SDL_Delay(3000);
				gameOver = true;
			}
		}
	}

	for (Boss* currentBoss : bosslist)
	{
		currentBoss->MoveDown(bossSpeed);

		//at end of game stop all enemies
		if (lives == 0)
		{
			currentBoss->StopMoving();
		}

		//respawn the enemy at the top of the screen if the player misses it and
		//lower the players number of lives by one 
		if (currentBoss->ypos > 896)
		{
			currentBoss->ypos = -256;
			currentBoss->xpos = 277;
			lives--;
			cout << "Lives left: " << lives - 1 << endl;

			//end of game, show final score
			if (lives == 0)
			{
				cout << "Game Over!" << endl;
				cout << "Final Score: " << score << endl;
				score = 0;
				SDL_Delay(3000);
				gameOver = true;
			}
		}
	}
}


void Game::render()
{
	// set background color
	//SDL_SetRenderDrawColor(renderer, 0, 204, 0, 255);

	

	// clear previous frame
	SDL_RenderClear(renderer);

	background->Render();

	restart->Render();

	for (Boss* currentBoss : bosslist)
	{
		currentBoss->Render();
	}

	//render enemies from vector
	for (Enemy* currentEnemy : enemylist)
	{
		currentEnemy->Render();
	}


	if (player->hasFired())
	{
		explosion->Render();
	}


	//render player
	player->Render();

	for (Enemy* currentEnemy : enemylist)
	{
		if (currentEnemy->isPointInside(player->xpos + 32, player->ypos + 32))
		{
			redCrosshair->Render();
		}
	}

	for (Boss* currentBoss : bosslist)
	{
		if (currentBoss->isPointInside(player->xpos + 32, player->ypos + 32))
		{
			redCrosshair->Render();
		}
	}

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
	delete explosion;
	delete restart;
	delete background;
	delete enemy;
	delete redCrosshair;
	delete boss;

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