#ifndef PLAYER_H
#define PLPAYER_H

#include "Sprite.h"
#include "globals.h"

class Graphics;

class Player : public Sprite
{
public:
	Player();
	Player(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	SDL_Rect playerBoxCollider;

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

private:
	float dx, dy;
};

#endif
