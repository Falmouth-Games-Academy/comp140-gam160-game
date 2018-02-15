#pragma once
#include <SDL_events.h>
#include <SDL_keyboard.h>

class InputManager {
public:
	InputManager();

	void Update();

	bool IsInputDown(SDL_Keycode keyCode) const;
	bool IsInputBooped(SDL_Keycode keyCode) const;

	void OnInputEvent(const SDL_Event& event);

private:
	static const int maxKeycodes = 256;
	enum KeyState {
		KeyUp = 0,
		KeyDown = 1,
		KeyBooped = 2,
		KeyUnbooped = 3,
	};

	KeyState keyStates[maxKeycodes];
};