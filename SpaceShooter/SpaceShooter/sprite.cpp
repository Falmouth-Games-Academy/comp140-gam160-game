#include "sprite.h"
#include "graphics.h"
#include "globals.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
	x(posX),
	y(posY)
{
	sourceRect.x = sourceX;
	sourceRect.y = sourceY;
	sourceRect.w = width;
	sourceRect.h = height;

	destRect.x = x;
	destRect.y = y;
	destRect.w = width;
	destRect.h = height;

	sprite = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadSprite(filePath));
	if (sprite == NULL)
	{
		printf("\nError: Unable to laod image\n");
	}
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics &graphics, float x, float y)
{
	//SDL_Rect destinationRectangle = { x, y, sourceRect.w * globals::SPRITE_SCALE, sourceRect.h * globals::SPRITE_SCALE };
	destRect.x = x;
	destRect.y = y;

	sourceRect.w = destRect.w;
	sourceRect.h = destRect.h;
	graphics.blitSurface(sprite, &sourceRect, &destRect);

}

bool Sprite::checkCollision(Sprite * sprite)
{
	//SDL_Rect destinationRectangle = { x, y, sourceRect.w * globals::SPRITE_SCALE, sourceRect.h * globals::SPRITE_SCALE };
	//SDL_Rect otherRect = { sprite->x, sprite->y, sprite->sourceRect.w * globals::SPRITE_SCALE, sprite->sourceRect.h * globals::SPRITE_SCALE };
	return SDL_HasIntersection(&destRect, &sprite->destRect);

}

void Sprite::update() {}