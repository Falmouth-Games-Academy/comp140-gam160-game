#ifndef GRAPHICS_H
#define GRAPHICS_H

/*
* Graphics
* Holds all info dealing with Graphics
*/

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>

struct SDL_Window;
struct SDL_Renderer;


class Graphics
{
public:
	Graphics();
	~Graphics();

	void UpdateGraphics();

	SDL_Texture* LoadSprite(std::string file);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* sprite;

};

#endif