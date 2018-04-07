#include "graphics.h"
#include "globals.h"

Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Space Shooter");
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

SDL_Surface* Graphics::loadSprite(const std::string &filePath)
{
	if (sprite.count(filePath) == 0)
	{
		sprite[filePath] = IMG_Load(filePath.c_str());
	}
	return sprite[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* DestinationRectangle)
{
	SDL_RenderCopy(renderer, texture, sourceRectangle, DestinationRectangle);
}

void Graphics::flip()
{
	SDL_RenderPresent(renderer);
}

void Graphics::clear()
{
	SDL_RenderClear(renderer);
}

SDL_Renderer* Graphics::getRenderer() const
{
	return renderer;
}