#include "enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(Graphics & graphics, float x, float y) :
	Sprite(graphics, "content/enemy.png", 0, 0, 110, 136, x, y)
{

}

void Enemy::draw(Graphics & graphics)
{
	Sprite::draw(graphics, x, y);
}

void Enemy::update(float elapsedTime)
{

	y += 0.2 * elapsedTime;

	if (y > globals::SCREEN_HEIGHT)
	{
		respawn();
		//std::cout << "Bullet was disabled" << std::endl;
	}
}

void Enemy::respawn()
{
	x = rand()%globals::SCREEN_WIDTH + 1;
	y = (rand()%350)*-1;
}
