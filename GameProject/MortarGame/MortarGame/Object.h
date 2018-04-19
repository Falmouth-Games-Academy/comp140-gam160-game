#pragma once
#include "SerialInterface.h"
#include "Game.h"

class Object 
{
public:
	Object(const char* texturesheet, int x, int y);
	Object();
	~Object();

	void Update();
	void Render();


private:

	int xpos;
	int ypos;

	int lastRot1 = 5000;
	int thisRot1;
	int swtch1;
	int swtch2;

	SDL_Texture* objTexture;
	SDL_Rect sourceRect, destRect;
	

	SerialInterface* serialInterface;
};