#include "Input.h"

const int InputManager::keycodeTranslators[InputManager::numExtraKeycodes] = 
	{SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_LSHIFT, SDLK_RSHIFT, SDLK_LCTRL, SDLK_RCTRL};

InputManager::InputManager() {
	return; // YOU HAD NONE JOB
}

void InputManager::Update() {
	// Update key states
	for (KeyState& state : keyStates) {
		if (state == KeyBooped) {
			state = KeyDown;
		} else if (state == KeyUnbooped) {
			state = KeyUp;
		}
	}

	// Update mouse states
	for (KeyState& state : mouseStates) {
		if (state == KeyBooped) {
			state = KeyDown;
		} else if (state == KeyUnbooped) {
			state = KeyUp;
		}
	}

	// Reset mouse scroll
	mouseScroll = 0;
}

bool InputManager::IsKeyDown(SDL_Keycode keyCode) const {
	keyCode = TranslateKeycode(keyCode);
	return keyStates[keyCode] == KeyDown || keyStates[keyCode] == KeyBooped;
}

bool InputManager::IsKeyBooped(SDL_Keycode keyCode) const {
	keyCode = TranslateKeycode(keyCode);
	return keyStates[keyCode] == KeyBooped;
}

bool InputManager::IsMouseDown(MouseButton button) const {
	return mouseStates[button] == KeyDown || mouseStates[button] == KeyBooped;
}

bool InputManager::IsMouseBooped(MouseButton button) const {
	return mouseStates[button] == KeyBooped;
}

bool InputManager::IsMouseUnbooped(MouseButton button) const {
	return mouseStates[button] == KeyUnbooped;
}

Vec2 InputManager::GetMousePosition() const {
	return mousePosition;
}

int InputManager::GetMouseScroll() const {
	return mouseScroll;
}

void InputManager::OnInputEvent(const SDL_Event& event) {
	switch (event.type) {
		// Keyboard input
		case SDL_KEYDOWN:
		case SDL_KEYUP: {
			SDL_Keycode keyCode = TranslateKeycode(event.key.keysym.sym);

			if (keyCode >= maxKeycodes) {
				return; // Key is too high and mighty to exist
			}

			if (event.key.repeat) {
				return; // Not interested in repeat events
			}

			if (event.type == SDL_KEYDOWN) {
				if (keyStates[keyCode] != KeyDown) {
					keyStates[keyCode] = KeyBooped;
				}
			} else if (event.type == SDL_KEYUP) {
				if (keyStates[keyCode] != KeyUp) {
					keyStates[keyCode] = KeyUnbooped;
				}
			}
			break;
		}
		// Mouse button input
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP: {
			if (event.button.button - 1 < NumMouseButtons) {
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (mouseStates[event.button.button - 1] != KeyDown) {
						mouseStates[event.button.button - 1] = KeyBooped;
					}
				} else if (event.type == SDL_MOUSEBUTTONUP) {
					if (mouseStates[event.button.button - 1] != KeyUp) {
						mouseStates[event.button.button - 1] = KeyUnbooped;
					}
				}
			}
			break;
		}
		// Mouse movement
		case SDL_MOUSEMOTION: {
			mousePosition = Vec2((float)event.motion.x, (float)event.motion.y); // profilethis
			break;
		}
		// Mouse scrolling
		case SDL_MOUSEWHEEL: {
			mouseScroll = event.wheel.y;
			break;
		}
	}
}