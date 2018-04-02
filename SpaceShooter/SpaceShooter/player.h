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


	//Moves player left by -dx
	void moveLeft();

	//Moves player right by dx
	void moveRight();

	//Moves player left by -dx
	void moveUp();

	//Moves player right by dx
	void moveDown();

	//Stops the player from moving
	void stopMoving();

private:
	float dx, dy;
};

#endif
