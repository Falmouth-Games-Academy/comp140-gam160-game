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

	SDL_Surface * loadSprite(const std::string & filePath);

	void blitSurface(SDL_Texture * texture, SDL_Rect * sourceRectangle, SDL_Rect * DestinationRectangle);

	void flip();

	void clear();

	SDL_Renderer * getRenderer() const;

	void UpdateGraphics();

	//SDL_Texture* LoadSprite(std::string file);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	std::map<std::string, SDL_Surface*> sprite;

};

#endif