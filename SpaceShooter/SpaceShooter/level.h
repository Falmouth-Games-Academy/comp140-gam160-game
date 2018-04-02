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

	SDL_Rect levelBoxCollider = { 0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT };

private:
	float dx, dy;

};

#endif