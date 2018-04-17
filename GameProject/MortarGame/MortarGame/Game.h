#pragma once
#include "SDL.h"
#include <stdio.h>
#include "SerialInterface.h"
#include "SDL_image.h"

class Game
{
public:
	Game();
	~Game();

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();

	void update();

	void render();
	
	void clean();

	bool running()
	{
		return isRunning;
	};

	static SDL_Renderer *renderer;
	static SDL_Event event;

	int lastRot1 = 5000;
	int thisRot1;

private:

	int cnt = 0;

	SDL_Window * mainWindow;


	bool isRunning;

	SerialInterface* serialInterface; //should really be put after SDL runs

};