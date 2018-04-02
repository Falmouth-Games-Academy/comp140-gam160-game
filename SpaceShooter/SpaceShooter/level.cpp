#include "level.h"
#include "graphics.h"
//#include "collisionBox.h"

namespace level_constants
{
	const float MOVE_SPEED = 0.5f;
}

Level::Level() {}

// intiates the level in a certain position with a sprite
Level::Level(Graphics &graphics, float x, float y) :
	Sprite(graphics, "content/SpaceBackground.jpg", 0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, x, y)
{
	graphics.loadSprite("content/SpaceBackground.jpg");
}

void Level::update(float elapsedTime)
{
	y += dy * elapsedTime;
	x += dx * elapsedTime;
}

void Level::draw(Graphics & graphics)
{
	Sprite::draw(graphics, x, y);
}