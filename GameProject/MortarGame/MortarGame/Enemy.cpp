#include "Object.h"
#include "Enemy.h"


Enemy::Enemy()
{

}

Enemy::Enemy(const char * texturesheet, float x, float y) :Object(texturesheet, x, y)
{

}

//
void Enemy::Update()
{
	Object::Update();
}

//function to move enemy down the screen
void Enemy::MoveDown(float speed)
{
	Object::Update();

	ypos = ypos + speed;
}


//function to respawn and stop the enemy
void Enemy::StopMoving()
{
	Object::Update();

	ypos = -64;
}


Enemy::~Enemy()
{
}
