#include "Boss.h"
#include "Object.h"



Boss::Boss()
{

}

Boss::Boss(const char * texturesheet, float x, float y) :Object(texturesheet, x, y)
{

}


void Boss::Update()
{
	Object::Update();

	sourceRect.h = 256;
	sourceRect.w = 256;
	sourceRect.x = 0;
	sourceRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.h = sourceRect.h;
	destRect.w = sourceRect.w;
}

//function to move enemy down the screen
void Boss::MoveDown(float speed)
{
	Object::Update();

	ypos = ypos + speed;
}


//function to respawn and stop the enemy
void Boss::StopMoving()
{
	Object::Update();

	ypos = -256;
}


Boss::~Boss()
{
}
