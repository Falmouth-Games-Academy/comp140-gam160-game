#pragma once
#include "Containers.h"
#include "Types.h"
#include "Math.h" // Vec3

namespace FMOD {class System; class Sound; class Channel;};

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
	SoundEmitter(const char* soundFilename, const Vec3& position, float32 lifeTimer = 0.0f, bool8 looping = false);
	~SoundEmitter();

public:
	// Plays the sound, with an optional start position
	void Play(uint32 startPositionInMs = 0);

	// Stops the sound, if it's playing
	void Stop();

public:
	// Sets the volume of the sound
	void SetVolume(float volume);

	// Sets the pitch (frequency) of the sound
	void SetPitch(float pitch);

private:
	// Location of the sound emitter, in world space
	Vec3 position;

	// Effects
	float32 pitch = 1.0f;
	float32 volume = 1.0f;

	// Whether or not the sound will loop
	bool8 isLooping = false;

	// Amount of seconds until the sound emitter is destroyed. If 0, the timer doesn't count down.
	float32 deathTimer = 0.0f;

	// Active FMOD channel used for this sound
	FMOD::Channel* channel = nullptr;

	// Sound effect used for this sound
	FMOD::Sound* sound = nullptr;
};