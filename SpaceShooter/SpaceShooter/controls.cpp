#include <SDL.h>
#include <iostream>

#include "controls.h"
#include "graphics.h"
#include "globals.h"
#include "player.h"
#include "input.h"

Controls::Controls() 
{
	//server.connectToServer("http://192.168.43.26:3000");
	
	if (serial.connect == true)
	{
		controllerActive = true;
	}
	
	else if (server.isConnected())
	{
		serverActive = true;
	}

	else
	{
		controllerActive = false;
		serverActive = false;
	}
}

Controls::~Controls() {}

void Controls::playerControls(Player &player, Input &input, Graphics &graphics)
{

	// Controller Controls
	if (controllerActive == true)
	{
		serial.newPos();
		serial.getButton();
		//std::cout << player.MOVE_SPEED << std::endl;

		//---------------------------------------------------------------------
		// SHOOTING
		//---------------------------------------------------------------------
		if (serial.button2 == 0)
		{
			std::cout << "Button2 works" << std::endl;
			player.shoot2(graphics);
		}
		if (serial.button1 == 0)
		{
			std::cout << "BUtton1 works" << std::endl;
			player.shoot1(graphics);
		}
		
		//initially stop the player before moveing, helps with not getting stuck in a movement loop
		player.stopMovingX();
		player.stopMovingY();

		//---------------------------------------------------------------------
		// MOVEMENT
		//---------------------------------------------------------------------
		// X movement code
		if (-3 > serial.x > 3)
		{
			//player.stopMovingY();	
		}

		else if (serial.x < -3)
		{
			//player.MOVE_SPEED = serial.x * player.MAX_MOVE_SPEED;
			player.moveRight();
		}

		else if (serial.x > 3)
		{
			//player.MOVE_SPEED = serial.x * player.MAX_MOVE_SPEED;
			player.moveLeft();
		}

		// Y movement code
		if (-3 > serial.y > 3)
		{
			//player.stopMovingY();
		}

		else if (serial.y < -3)
		{
			player.moveUp();
		}

		else if (serial.y > 3)
		{
			player.moveDown();
		}
		
	}

	// Keyboard Controls
	if (!controllerActive)
	{
		
		//---------------------------------------------------------------------
		// MOVEMENT
		//---------------------------------------------------------------------
		if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true || input.isKeyHeld(SDL_SCANCODE_A) == true)
		{
			player.moveLeft();
		}

		if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true || input.isKeyHeld(SDL_SCANCODE_D) == true)
		{
			player.moveRight();
		}

		if (input.isKeyHeld(SDL_SCANCODE_UP) == true || input.isKeyHeld(SDL_SCANCODE_W) == true)
		{
			player.moveUp();
		}

		if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true || input.isKeyHeld(SDL_SCANCODE_S) == true)
		{
			player.moveDown();
		}

		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) &&
			!input.isKeyHeld(SDL_SCANCODE_RIGHT) &&
			!input.isKeyHeld(SDL_SCANCODE_A) &&
			!input.isKeyHeld(SDL_SCANCODE_D))
		{
			player.stopMovingX();
		}

		if (!input.isKeyHeld(SDL_SCANCODE_UP) &&
			!input.isKeyHeld(SDL_SCANCODE_W) &&
			!input.isKeyHeld(SDL_SCANCODE_DOWN) &&
			!input.isKeyHeld(SDL_SCANCODE_S))
		{
			player.stopMovingY();
		}

		//---------------------------------------------------------------------
		// SHOOTING
		//---------------------------------------------------------------------
		if (input.wasKeyPressed(SDL_SCANCODE_Q) == true)
		{
			player.shoot1(graphics);
		}

		if (input.wasKeyPressed(SDL_SCANCODE_E) == true)
		{
			player.shoot2(graphics);
		}
	}
}