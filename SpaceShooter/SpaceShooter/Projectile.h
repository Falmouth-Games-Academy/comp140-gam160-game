#pragma once

#include "Sprite.h"
#include "globals.h"

class Graphics;

class Projectile : public Sprite
{
public:
	Projectile();
	Projectile(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	float moveSpeed = 0.6;

	void shootUp();

	void disableProjectile();

	bool isActive = false;

	void spawnPos(float Spawnx, float Spawnxy);


private:
	float px, py;
};
