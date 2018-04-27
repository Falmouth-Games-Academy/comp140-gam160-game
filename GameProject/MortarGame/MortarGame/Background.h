#pragma once

#include <SDL.h>


class Background
{
public:
	Background(const char* texturesheet, int x, int y);
	Background();
	~Background();

	void Render();

	virtual void Update();


	int xpos;
	int ypos;


	SDL_Texture* objTexture;
	SDL_Rect sourceRect, destRect;

};