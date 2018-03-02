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

	// Translates an Unusually High Number (UHN) SDL keycode (such as shift, space, etc) to one that can fit in the keyStates array, if known
	inline static int TranslateKeycode(SDL_Keycode keyCode);

private:
	static const int numExtraKeycodes = 8;
	static const int numNormalKeycodes = 256;
	static const int maxKeycodes = numNormalKeycodes + numExtraKeycodes;

	static const int keycodeTranslators[numExtraKeycodes];

	// Key and mouse states
	KeyState keyStates[maxKeycodes];
	KeyState mouseStates[NumMouseButtons];

	Vec2 mousePosition; // Mouse position, in pixels, relative to active window

	int mouseScroll; // Length of mouse scroll this frame, in unknown units
};

inline int InputManager::TranslateKeycode(SDL_Keycode keyCode) {
	if (keyCode <= numNormalKeycodes)
		return keyCode; // This is already in the array

	for (int i = 0; i < sizeof (keycodeTranslators) / sizeof (keycodeTranslators[0]); ++i) {
		if (keyCode == keycodeTranslators[i]) {
			return numNormalKeycodes + i;
		}
	}

	return 0;
}