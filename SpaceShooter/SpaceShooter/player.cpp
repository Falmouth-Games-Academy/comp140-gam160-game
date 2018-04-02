#include <iostream>

#include "player.h"
#include "graphics.h"
#include "collisionBox.h"

namespace player_constants
{
	const float MOVE_SPEED = 0.5f;
}

Player::Player() {}

// intiates the player in a certain position with a sprite
Player::Player(Graphics &graphics, float x, float y) : 
	Sprite(graphics, "content/SpaceShip.png", 0, 0, 128, 128, x, y)
{
	graphics.loadSprite("content/SpaceShip.png");
}

// All the movement functions
void Player::moveLeft()
{
	dx = -player_constants::MOVE_SPEED;
}

void Player::moveRight()
{
	dx = +player_constants::MOVE_SPEED;
}

void Player::moveUp()
{
	dy = -player_constants::MOVE_SPEED;
}

void Player::moveDown()
{
	dy = +player_constants::MOVE_SPEED;
}

void Player::stopMovingX()
{
	dx = 0.0f;
}

void Player::stopMovingY()
{
	dy = 0.0f;
}

// updates the player's x y positions
void Player::update(float elapsedTime)
{
	y += dy * elapsedTime;
	x += dx * elapsedTime;
}

// updates the player's sprite position on screen
void Player::draw(Graphics & graphics)
{
	Sprite::draw(graphics, x, y);
}


void Player::levelCollisions(SDL_Rect A, SDL_Rect B)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = x;
	rightA = x + A.w;
	topA = y;
	bottomA = y + A.h;

	//Calculate the sides of rect B
	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;

	//If any of the sides from A are outside of B
	if (bottomA >= bottomB)
	{
		stopMovingY();
		y -= 1;
	}

	if (topA <= topB)
	{
		stopMovingY();
		y += 1;
	}

	if (rightA >= rightB)
	{
		stopMovingX();
		x -= 1;
	}

	if (leftA <= leftB)
	{
		stopMovingX();
		x += 1;
	}
}

