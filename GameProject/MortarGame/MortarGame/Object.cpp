#include "Object.h"
#include "GraphicsManager.h"

Object::Object(const char* texturesheet, int x, int y)
{
	objTexture = GraphicsManager::LoadImage(texturesheet);

	xpos = x;
	ypos = y;

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
	//xpos++;
	//ypos++;

	sourceRect.h = 64;
	sourceRect.w = 64;
	sourceRect.x = 0;
	sourceRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.h = sourceRect.h * 2;
	destRect.w = sourceRect.w * 2;
	/*
	serialInterface->getValues();
	thisRot1 = serialInterface->getRot1();

	if (thisRot1 > lastRot1)
	{
		ypos++;
	}

	if (thisRot1 < lastRot1)
	{
		ypos--;
	}

	lastRot1 = serialInterface->getRot1();
	*/
}

void Object::Render()
{
	SDL_RenderCopy(Game::renderer, objTexture, &sourceRect, &destRect);

}