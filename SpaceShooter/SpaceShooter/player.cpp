#include "player.h"
#include "graphics.h"

namespace player_constants
{
	const float MOVE_SPEED = 0.2f;
}

Player::Player() {}

Player::Player(Graphics &graphics, float x, float y)
{
	//graphics.loadImage("content\SpaceShip.png");
}

void Player::moveLeft()
{
	dx = -player_constants::MOVE_SPEED;
}

void Player::moveRight()
{
	dx = +player_constants::MOVE_SPEED;
}

void Player::stopMoving()
{
	dx = 0.0f;
}

void Player::update(float elapsedTime)
{
	dx += dx * elapsedTime;

}