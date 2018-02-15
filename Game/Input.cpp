#include "Input.h"

InputManager::InputManager() {
	return; // YOU HAD NONE JOB
}

void InputManager::Update() {
	for (KeyState& state : keyStates) {
		if (state == KeyBooped) {
			state = KeyDown;
		} else if (state == KeyUnbooped) {
			state = KeyUp;
		}
	}
}

bool InputManager::IsInputDown(SDL_Keycode keyCode) const {
	return keyStates[keyCode] == KeyDown || keyStates[keyCode] == KeyBooped;
}

bool InputManager::IsInputBooped(SDL_Keycode keyCode) const {
	return keyStates[keyCode] == KeyBooped;
}

void InputManager::OnInputEvent(const SDL_Event& event) {
	SDL_Keycode keyCode = event.key.keysym.sym;

	if (event.key.keysym.sym >= maxKeycodes) {
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
}
