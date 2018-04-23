#include "Object.h"
#include "Enemy.h"


Enemy::Enemy()
{

}

Enemy::Enemy(const char * texturesheet, int x, int y) :Object(texturesheet, x, y)
{

}

void Enemy::Update()
{
	Object::Update();

	xpos--;

}

void Enemy::MoveDown()
{
	Object::Update();

	ypos++;

}

void Enemy::MoveDown2()
{
	Object::Update();

	ypos = ypos + 2;

}

void Enemy::MoveDown3()
{
	Object::Update();

	ypos = ypos + 3;

}

void Enemy::StopMoving()
{
	Object::Update();

	ypos = 0;

}

void Enemy::MoveLeft()
{
	Object::Update();

	xpos--;

}

Enemy::~Enemy()
{
}
