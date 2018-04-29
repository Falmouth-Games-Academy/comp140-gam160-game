#include <iostream>

#include "Projectile.h"
#include "graphics.h"

Projectile::Projectile() {}

Projectile::Projectile(Graphics &graphics, float x, float y) :
	Sprite(graphics, "content/projectile.png", 0, 0, 26, 26, x, y)
{
	graphics.loadSprite("content/projectile.png");
}

// updates the projectile's x y positions
void Projectile::update(float elapsedTime)
{
	if (isActive == true)
	{
		shootUp();

		y += py * elapsedTime;

		if (y < -28)
		{
			disableProjectile();
			std::cout << "Bullet was disabled" << std::endl;
		}
	}
}

// updates the player's sprite position on screen
void Projectile::draw(Graphics & graphics)
{
	if (isActive == true)
		Sprite::draw(graphics, x, y);
}

//spawns and shoots the projectile forwards
void Projectile::shootUp()
{
	py = -moveSpeed;
}

void Projectile::disableProjectile()
{
	isActive = false;
}

void Projectile::spawnPos(float Spawnx, float Spawny)
{
	x = Spawnx;
	y = Spawny;

	isActive = true;
}