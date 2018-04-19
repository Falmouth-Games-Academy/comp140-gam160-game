#include "Object.h"
#include "GraphicsManager.h"


Object::Object(const char* texturesheet, int x, int y)
{
	objTexture = GraphicsManager::LoadImage(texturesheet);

	xpos = x;
	ypos = y;
	serialInterface = new SerialInterface();
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
	

	serialInterface->getValues();
	thisRot1 = serialInterface->getRot1();
	swtch1 = serialInterface->getSwtch1();
	swtch2 = serialInterface->getSwtch2();
	
	if (swtch2 == 0)
	{
		if (thisRot1 > lastRot1)
		{
			ypos = ypos + 5;
		}

		if (thisRot1 < lastRot1)
		{
			ypos = ypos - 5;
		}

		lastRot1 = serialInterface->getRot1();
	}

	if (swtch2 == 1)
	{
		if (thisRot1 > lastRot1)
		{
			xpos = xpos + 5;
		}

		if (thisRot1 < lastRot1)
		{
			xpos = xpos - 5;
		}

		lastRot1 = serialInterface->getRot1();
	}

	if (swtch1 == 1)
	{
		xpos = xpos + 30;

		
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

void Object::Render()
{
	SDL_RenderCopy(Game::renderer, objTexture, &sourceRect, &destRect);

}