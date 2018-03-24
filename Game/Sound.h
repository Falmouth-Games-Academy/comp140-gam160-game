#pragma once
#include "Types.h"

namespace FMOD {class System;};

class SoundManager {
public:
	bool Init();
	void Shutdown();

private:
	FMOD::System *system = nullptr;
};

class SoundEmitter {
public:


private:
	// Whether or not the sound will loop
	bool8 isLooping;

	// Amount of seconds until the sound emitter is destroyed
	float32 deathTimer;
};