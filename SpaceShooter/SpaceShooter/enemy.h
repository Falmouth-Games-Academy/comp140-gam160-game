#pragma once
#include <vector>

#include "Sprite.h"
#include "globals.h"

class Graphics;

class Enemy : public Sprite
{
public:
	Enemy();
	Enemy(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	void respawn();

};