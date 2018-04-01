#ifndef PLAYER_H
#define PLPAYER_H

#include "globals.h"

class Graphics;

class Player
{
public:
	Player();
	Player(Graphics &graphics, float x, float y);
	void update(float elapsedTime);

	//Moves player left by -dx
	void moveLeft();

	//Moves player right by dx
	void moveRight();

	//Moves player left by -dx
	void moveLeftC();

	//Moves player right by dx
	void moveRightC();

	//Stops the player from moving
	void stopMoving();

	//Stops the player from moving
	void stopMovingC();

private:
	float dx, dy;
};

#endif
