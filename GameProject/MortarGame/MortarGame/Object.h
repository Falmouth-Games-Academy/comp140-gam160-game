#pragma once

#include "SerialInterface.h"
#include <SDL.h>


class Object
{
public:
	Object(const char* texturesheet, float x, float y);
	Object();
	~Object();

	void Render();
	virtual void Update();

	SerialInterface* serialInterface;

	bool isPointInside(float x, float y);



	float xpos;
	float ypos;

	int lastRot1 = 5000;
	int thisRot1;
	int swtch1;
	int swtch2;

	SDL_Texture* objTexture;
	SDL_Rect sourceRect, destRect;


	//SerialInterface* serialInterface;
};