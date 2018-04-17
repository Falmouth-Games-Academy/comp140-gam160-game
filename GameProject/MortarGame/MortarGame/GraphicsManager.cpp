#include "GraphicsManager.h"

SDL_Texture* GraphicsManager::LoadImage(const char* image)
{
	SDL_Surface* tempSurface = IMG_Load(image);
	SDL_Texture* img = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return img;
}