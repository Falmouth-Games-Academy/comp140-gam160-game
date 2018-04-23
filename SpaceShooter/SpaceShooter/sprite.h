#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>

class Graphics;

/* Sprite class
* Holds all the information for individual sprites
*/

class Sprite
{
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);

	virtual ~Sprite();
	virtual void update();
	void draw(Graphics &graphics, float x, float y);

	SDL_Rect destRect;
	SDL_Rect sourceRect;
	SDL_Texture* sprite;

	float x, y;

	bool checkCollision(Sprite* sprite);
};

#endif // !SPRITE_H

