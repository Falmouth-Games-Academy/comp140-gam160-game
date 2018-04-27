#include "Background.h"
#include "GraphicsManager.h"


Background::Background(const char* texturesheet, int x, int y)
{
	objTexture = GraphicsManager::LoadImage(texturesheet);

	xpos = x;
	ypos = y;


}

Background::Background()
{
	xpos = 0;
	ypos = 0;
	objTexture = nullptr;



}

Background::~Background()
{
	SDL_DestroyTexture(objTexture);
}

void Background::Update()
{


	sourceRect.h = 640;
	sourceRect.w = 800;
	sourceRect.x = 0;
	sourceRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.h = sourceRect.h;
	destRect.w = sourceRect.w;

}



void Background::Render()
{
	//SDL_SetTextureColorMod(objTexture, 255, 0, 0);
	SDL_RenderCopy(Game::renderer, objTexture, &sourceRect, &destRect);
}