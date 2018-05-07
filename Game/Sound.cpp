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
	// Update FMOD system
	system->update();
}

FMOD::System* SoundManager::GetSystem() {
	return system;
}

SoundEmitter::SoundEmitter(const char* soundFilename, const Vec3& position, float32 lifeTime, bool8 looping) {
	// Load the sound file from the cache
	FMOD_RESULT error = game.GetSound().GetSystem()->createSound(soundFilename, FMOD_2D, nullptr, &sound);

	if (error != FMOD_OK) {
		// Sound not successfully loaded
		return;
	}
}

SoundEmitter::~SoundEmitter() {
	// Free FMOD resources
	if (sound) {
		sound->release();
		sound = nullptr;
	}

	channel = nullptr;
}

void SoundEmitter::Play(uint32 startPosition) {
	// Disable the active sound channel if there is one
	Stop();

	// Create the new sound channel
	if (sound) {
		game.GetSound().GetSystem()->playSound(sound, nullptr, false, &channel);
		channel->setPitch(pitch);
		channel->setVolume(volume);
	}

	// Play the sound channel (if it worked)
	if (channel) {
		channel->setPosition(startPosition, FMOD_TIMEUNIT_MS);
		channel->setPaused(false);
	}
}

void SoundEmitter::Stop() {
	if (channel) {
		channel->stop();
		channel = nullptr;
	}
}

void SoundEmitter::SetVolume(float value) {
	// Some really basic wrapping here...
	if (channel) {
		channel->setVolume(this->volume = value);
	}
}

void SoundEmitter::SetPitch(float pitch) {
	// Do thewrap rap yo, this is encapsulated fo' sho, so you better know, I just realised I got sunburn on the back of my hands, darn!! Knew I shouldn't have gone outside!
	if (channel) {
		channel->setPitch(this->pitch = pitch);
	}
}
