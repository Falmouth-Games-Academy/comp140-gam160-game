#pragma once
#include "Containers.h"
#include "Types.h"
#include "Math.h" // Vec3

namespace FMOD {class System;};

class SoundManager {
public:
	bool Init();
	void Shutdown();

	void Update();

	FMOD::System* GetSystem();

private:
	FMOD::System *system = nullptr;

	Array<class SoundEmitter> emitters;
};

class SoundEmitter {
public:
	SoundEmitter(const Vec3& position, float32 lifeTimer, bool8 looping);

private:
	// Location of the sound emitter, in world space
	Vec3 position;

	// Whether or not the sound will loop
	bool8 isLooping = false;

	// Amount of seconds until the sound emitter is destroyed. If 0, the timer doesn't count down.
	float32 deathTimer = 0.0f;
};