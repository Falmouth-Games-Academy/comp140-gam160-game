#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <map>

class Input {
public:
	void beginNewFrame();
	void KeyUpEvent(const SDL_Event& event);
	void KeyDownEvent(const SDL_Event& event);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);

private:
	std::map<SDL_Scancode, bool> heldKeys;
	std::map<SDL_Scancode, bool> pressedKeys;
	std::map<SDL_Scancode, bool> releasedKeys;
};

#endif
