#include <iostream>

#include "player.h"
#include "graphics.h"

namespace player_constants
{
	const float MAX_MOVE_SPEED = 0.8f;
	 float MOVE_SPEED = 0.5f;
}

Player::Player() {}

// intiates the player in a certain position with a sprite
Player::Player(Graphics &graphics, float x, float y) : 
	Sprite(graphics, "content/SpaceShip.png", 0, 0, 128, 128, x, y)
{
	graphics.loadSprite("content/SpaceShip.png");

	playerBoxCollider.w = sourceRect.w;
	playerBoxCollider.h = sourceRect.h;
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

// fast moveing


void Player::stopMovingX()
{
	dx = 0.0f;
}

void Player::stopMovingY()
{
	dy = 0.0f;
}

void Player::shoot1(Graphics &graphics)
{
	Projectile* newBullet1 = new Projectile(graphics, 100, 400);
	newBullet1->draw(graphics);
}

void Player::shoot2(Graphics &graphics)
{
	Projectile* newBullet2 = new Projectile(graphics, gun2.x, gun2.y);
}

// updates the player's x y positions
void Player::update(float elapsedTime)
{
	x += dx * elapsedTime;
	y += dy * elapsedTime;
	
	//setup gun positions for each frame
	gun1.x = x + 12;
	gun1.y = y + 12;

	gun2.x = x + 20;
	gun2.y = y + 20;

	//update bullet position
	bullet.update(elapsedTime);

	//resets the player's collider for each frame
	playerBoxCollider.x = x;
	playerBoxCollider.y = y;
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
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

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

