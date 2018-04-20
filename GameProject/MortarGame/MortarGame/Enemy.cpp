#include "Enemy.h"
#include "Object.h"



Enemy::Enemy()
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

void Enemy::MoveLeft()
{
	Object::Update();

	xpos--;

}

Enemy::~Enemy()
{
}
