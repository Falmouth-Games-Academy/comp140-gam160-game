#include "input.h"

//Input class
//Keeps track of keyboard state


//This function gets called at the begining of each frame
//to reset the keys that are no longer valid
void Input::beginNewFrame()
{
	pressedKeys.clear();
	releasedKeys.clear();
}

//This gets called when a key has been pressed
void Input::KeyDownEvent(const SDL_Event& event)
{
	pressedKeys[event.key.keysym.scancode] = true;
	heldKeys[event.key.keysym.scancode] = true;
}

//this gets called when a key is released
void Input::KeyUpEvent(const SDL_Event& event)
{
	releasedKeys[event.key.keysym.scancode] = true;
	heldKeys[event.key.keysym.scancode] = false;
}

//Check if a certain key was pressed during the current frame
bool Input::wasKeyPressed(SDL_Scancode key)
{
	return pressedKeys[key];
}

//Check if a certain key was released during the current frame
bool Input::wasKeyReleased(SDL_Scancode key)
{
	return releasedKeys[key];
}

//Check if a certain key is being held down
bool Input::isKeyHeld(SDL_Scancode key)
{
	return heldKeys[key];
}
