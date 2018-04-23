#include "Object.h"
#include "Enemy.h"


Enemy::Enemy()
{
	
}

Enemy::Enemy(const char * texturesheet, int x, int y):Object(texturesheet, x, y)
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
