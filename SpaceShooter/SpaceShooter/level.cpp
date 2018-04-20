#include <iostream>
#include "level.h"
#include "graphics.h"

namespace level_constants
{
	const float MOVE_SPEED = 0.05f;
}

Level::Level() {}

const std::string Level::listOfMaps[4] = { "content/SpaceBackground.png","content/SpaceBackground4.png","content/SpaceBackground3.png","content/SpaceBackground2.png" };
const std::string Level::listOf = "content/SpaceBackground.png";

// intiates the level in a certain position with a sprite
Level::Level(Graphics &graphics, float x, float y) : 
	Sprite(graphics, listOfMaps[rand() % 4], 0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, x, y)
{
	//graphics.loadSprite("content/SpaceBackground.png");
	//const std::string filePath = listOfMaps[rand() % 4];
}

void Level::update(float elapsedTime)
{
	if (movingBackground == true)
	{
		moveBackground();
	}
	if (y1 >= globals::SCREEN_HEIGHT)
	{
		y1 = 0 - globals::SCREEN_HEIGHT;
		//std::cout << y1 << std::endl;

	}
	if (y2 >= globals::SCREEN_HEIGHT)
	{
		y2 = 0 - globals::SCREEN_HEIGHT;
		//std::cout << y2 << std::endl;

	}
	y1 += dy * elapsedTime;
	y2 += dy * elapsedTime;
}

void Level::draw(Graphics &graphics)
{
	Sprite::draw(graphics, x1, y1);
	Sprite::draw(graphics, x2, y2);
}

void Level::moveBackground()
{
	dy = +level_constants::MOVE_SPEED;
}