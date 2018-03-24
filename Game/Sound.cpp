#include <objbase.h> // CoInitializeEx, required for FMOD

#include <fmod.hpp>
#include <fmod_errors.h>
#include <cstdio> // debug printing

#include "Sound.h"

bool SoundManager::Init() {
	// Initialise COM (required for FMOD Windows)
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// Initialise FMOD
	FMOD_RESULT result;

	// Create the system object
	result = FMOD::System_Create(&system);      // Create the main system object.
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		goto InitError; // omg, look at all this harmful I'm considering
	}

	// Perform initialisation
	result = system->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		goto InitError;
	}

	FMOD::Sound* imASound;
	result = system->createSound("./sounds/wilhelm.mp3", FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &imASound);

	FMOD::Channel* channel;
	system->playSound(imASound, nullptr, true, &channel);

	channel->setFrequency(44100.0f);
	channel->setPaused(false);

	// It worked!
	return true;

InitError:
	// It didn't work! Harm occured.
	CoUninitialize();

	return false;
}

void SoundManager::Shutdown() {
	CoUninitialize();
}