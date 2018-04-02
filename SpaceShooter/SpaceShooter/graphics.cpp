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

/*
Graphics::Graphics()
{
	//SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->window, &this->renderer);
	//SDL_SetWindowTitle(this->window, "Space Shooter");

	window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	
	SDL_SetRenderDrawColor(window, 255, 0, 0, 0);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_Image could not be initialised! SDL_image Error: %s\n", IMG_GetError());
	}
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(this->renderer);
}

void Graphics::UpdateGraphics()
{
	SDL_Rect screenRectangle = { 0, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT };
	SDL_Rect imageRectangle = { 0, 0, 128, 128 };

	// Clear the renderer
	SDL_RenderClear(renderer);

	// Render the Image
	SDL_RenderCopy(renderer, sprite, &screenRectangle, &imageRectangle);

	// Show the final render
	SDL_RenderPresent(renderer);

	sprite = LoadSprite("content/SpaceShip.png");
}

SDL_Texture* Graphics::LoadSprite(std::string file)
{
	SDL_Texture* newSprite = NULL;

	SDL_Surface* loadedSurface = IMG_Load(file.c_str());

	if (loadedSurface == NULL)
	{
		printf("Unable to load the image %s! SDL_image Error %s\n", file.c_str(), IMG_GetError);
	}
	else
	{
		newSprite = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		if (newSprite == NULL)
		{
			printf("Unable to create the sprite from %s! SDL_image Error %s\n", file.c_str(), IMG_GetError);
		}

		SDL_FreeSurface(loadedSurface);
	}
	return newSprite;
}
*/
