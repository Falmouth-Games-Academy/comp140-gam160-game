#include "Object.h"
#include "GraphicsManager.h"


Object::Object(const char* texturesheet, float x, float y)
{
	objTexture = GraphicsManager::LoadImage(texturesheet);

	xpos = x;
	ypos = y;

	serialInterface = nullptr;
}

Object::Object()
{
	xpos = 0;
	ypos = 0;
	objTexture = nullptr;

	



}

Object::~Object()
{
	SDL_DestroyTexture(objTexture);
}

void Object::Update()
{
	//gets the values from the arduino code
	if (serialInterface) {
		serialInterface->getValues();
		thisRot1 = serialInterface->getRot1();
		swtch1 = serialInterface->getSwtch1();
		swtch2 = serialInterface->getSwtch2();
	}

	sourceRect.h = 64;
	sourceRect.w = 64;
	sourceRect.x = 0;
	sourceRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.h = sourceRect.h;
	destRect.w = sourceRect.w;
}

bool Object::isPointInside(float x, float y)
{
	SDL_Point point;
	point.x = x;
	point.y = y;

	return SDL_PointInRect(&point, &destRect);
}

void Object::Render()
{
	//SDL_SetTextureColorMod(objTexture, 255, 0, 0);
	SDL_RenderCopy(Game::renderer, objTexture, &sourceRect, &destRect);
}