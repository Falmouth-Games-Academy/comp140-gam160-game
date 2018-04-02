#include "player.h"
#include "graphics.h"

namespace player_constants
{
	const float MOVE_SPEED = 0.5f;
}

Player::Player() {}

Player::Player(Graphics &graphics, float x, float y) : 
	Sprite(graphics, "content/SpaceShip.png", 0, 0, 128, 128, x, y)
{
	graphics.loadSprite("content/SpaceShip.png");
}

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

void Player::stopMoving()
{
	dx = 0.0f;
	dy = 0.0f;
}

void Player::update(float elapsedTime)
{
	y += dy * elapsedTime;
	x += dx * elapsedTime;
}

void Player::draw(Graphics & graphics)
{
	Sprite::draw(graphics, x, y);
}

