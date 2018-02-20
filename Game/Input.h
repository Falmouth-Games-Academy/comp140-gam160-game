#pragma once
#include <SDL_events.h>
#include <SDL_keyboard.h>

#include "Math.h" // Vec2

class InputManager {
public:
	enum KeyState {
		KeyUp = 0,
		KeyDown = 1,
		KeyBooped = 2,
		KeyUnbooped = 3,
	};

	enum MouseButton {
		LeftButton   = 0,
		MiddleButton = 1,
		RightButton  = 2,
		NumMouseButtons = 3,
	};

public:
	InputManager();

	void Update();

	bool IsKeyDown(SDL_Keycode keyCode) const;
	bool IsKeyBooped(SDL_Keycode keyCode) const;

	bool IsMouseDown(MouseButton button) const;
	bool IsMouseBooped(MouseButton button) const;
	bool IsMouseUnbooped(MouseButton button) const;

	int GetMouseScroll() const;

	Vec2 GetMousePosition() const;

	void OnInputEvent(const SDL_Event& event);

private:
	static const int maxKeycodes = 256;

	KeyState keyStates[maxKeycodes];
	KeyState mouseStates[NumMouseButtons];

	Vec2 mousePosition; // Mouse position, in pixels, relative to active window

	int mouseScroll; // Length of mouse scroll this frame, in unknown units
};