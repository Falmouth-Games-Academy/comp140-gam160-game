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
	shootUp();

	//x += px * elapsedTime;
	y += py * elapsedTime;

}

// updates the player's sprite position on screen
void Projectile::draw(Graphics & graphics)
{
	Sprite::draw(graphics, x, y);
}

//spawns and shoots the projectile forwards
void Projectile::shootUp()
{
	py = -moveSpeed;
}

void Projectile::deleteProjectile()
{
	//if()
}
