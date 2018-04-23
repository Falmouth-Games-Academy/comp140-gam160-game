#include "Object.h"
#include "Enemy.h"


Enemy::Enemy()
{

}

Enemy::Enemy(const char * texturesheet, int x, int y) :Object(texturesheet, x, y)
{

}

//
void Enemy::Update()
{
	Object::Update();
}

//function to move enemy down the screen
void Enemy::MoveDown()
{
	Object::Update();

	ypos++;
}

//function to move enemy down the screen faster
void Enemy::MoveDown2()
{
	Object::Update();

	ypos = ypos + 2;
}

//function to move enemy down the screen at maximum speed
void Enemy::MoveDown3()
{
	Object::Update();

	ypos = ypos + 3;
}

//function to respawn and stop the enemy
void Enemy::StopMoving()
{
	Object::Update();

	ypos = 0;
}


Enemy::~Enemy()
{
}
