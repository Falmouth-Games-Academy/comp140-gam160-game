#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "Projectile.h"
#include "level.h"

class Graphics;

class Game {
public:

	Game();
	~Game();
	bool running = true;

private:
	void gameLoop();
	void drawGraphics(Graphics &graphics);
	void update(float elapsedTime);

	Player player;
	Level level;
	Projectile projectile;

};

#endif // GAME_H
