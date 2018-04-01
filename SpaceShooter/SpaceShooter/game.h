#ifndef GAME_H
#define GAME_H

#include "player.h"

class Graphics;

class Game {
public:
	Game();
	~Game();
	bool running = true;

private:
	void gameLoop();
	void draw(Graphics &graphics);

	void update(float elapsedTime);

	Player player;

};



#endif // GAME_H
