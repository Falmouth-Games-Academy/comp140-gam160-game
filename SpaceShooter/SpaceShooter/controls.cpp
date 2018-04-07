#include <SDL.h>

#include "controls.h"
#include "globals.h"
#include "player.h"
#include "input.h"

void Controls::playerControls(Player &player, Input &input)
{
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
}