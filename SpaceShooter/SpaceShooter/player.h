#ifndef PLAYER_H
#define PLPAYER_H

#include <vector>

#include "Sprite.h"
#include "globals.h"
#include "Projectile.h"

class Graphics;
class Projectile;

class Player : public Sprite
{
public:
	Player();
	Player(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	SDL_Rect playerBoxCollider;
	Projectile bullet;

	void levelCollisions(SDL_Rect A, SDL_Rect B);

	// float MAX_MOVE_SPEED = 0.8f;
	//float MOVE_SPEED = 0.5f;

	// All the movement functions
	//Moves player left by -dx
	void moveLeft();

	//Moves player right by dx
	void moveRight();

	//Moves player left by -dx
	void moveUp();

	//Moves player right by dx
	void moveDown();

	//Stops the player from moving along the X axis
	void stopMovingX();

	//Stops the player from moving along the Y axis
	void stopMovingY();

	//shooting functions for the player to shoot
	void shoot1(Graphics &graphics);
	void shoot2(Graphics &graphics);

private:
	float dx, dy;

	struct gunPos
	{
		float x;
		float y;
	};

	gunPos gun1;
	gunPos gun2;

};

#endif
