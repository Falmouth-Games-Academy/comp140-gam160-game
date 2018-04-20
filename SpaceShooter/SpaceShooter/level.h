#ifndef LEVEL_H
#define LEVEL_H

#include "Sprite.h"
#include "globals.h"
class Graphics;

class Level : public Sprite
{
public:
	Level();
	Level(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	const static std::string listOfMaps[4];
	const static std::string listOf;


	SDL_Rect levelBoxCollider = { 0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT };

	bool movingBackground = true;

private:
	float dx = 0, dy = 0;
	float x1 = 0, y1 = 0;
	float x2 = 0, y2 = 0 -globals::SCREEN_HEIGHT;

	void moveBackground();

};

#endif