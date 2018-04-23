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
	result = system->init(32, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		goto InitError;
	}

	// Latency should be pretty low
	system->setDSPBufferSize(256, 2);

	// It worked!
	return true;

InitError:
	// It didn't work! Harm occured.
	CoUninitialize();

	return false;
}

void SoundManager::Shutdown() {
	// Cleanup
	if (system) {
		system->release();
	}

	CoUninitialize();
}

#include "Game.h"

void SoundManager::Update() {
	// Play blargh sound (very much debug)
	FMOD_RESULT result;
	static FMOD::Sound* imASound;
	static FMOD::Sound* recordedSound;
	static FMOD::Channel* channel;

	if (game.GetPlayer().GetLaserPower() > 0.0f) {
		if (!imASound) {
			result = system->createSound("./sounds/IntenseLaser.mp3", FMOD_2D, nullptr, &imASound);
		}

		if (!channel) {
			system->playSound(imASound, nullptr, true, &channel);

			if (channel) {
				channel->setPosition(50, FMOD_TIMEUNIT_MS);
				channel->setPaused(false);
			}
		}

		if (channel) {
			bool isPlaying = false;
			channel->isPlaying(&isPlaying);
			
			if (isPlaying) {
				channel->setVolume(game.GetPlayer().GetLaserPower());
				channel->setPitch(Math::randfloat(0.9f, 1.1f));
			}
		}
	} else {
		if (channel) {
			channel->stop();
			channel = nullptr;
		}
	}
}

FMOD::System* SoundManager::GetSystem() {
	return system;
}

SoundEmitter::SoundEmitter(const Vec3& position, float32 lifeTime, bool8 looping) {

}
