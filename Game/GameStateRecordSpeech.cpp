#include "GameStateRecordSpeech.h"
#include "GameStatePlay.h"
#include "Game.h"
#include "String.h" // Debug string rendering

#include <fmod.hpp>

struct SyllableKeyframe {
	// Position in the sound, in samples
	unsigned int position;

	// todo
};

unsigned int openPositions[20];
unsigned int closedPositions[20];
int numSwitchPositions = 0;
int currentSwitchPosition = -1;

bool isMouthOpen = false;
float upperOpenThreshold = 0;
float lowerClosedThreshold = 0;
float thresholdRange = 0.1f;

bool isRecording = false;
bool isPlaying = false;

FMOD::Channel* playingChannel = nullptr;

void GameStateRecordSpeech::Update(float deltaTime) {
	// Update gestures
	game.GetGesture().Update();

	// Update the player but lock their position
	Hand& player = game.GetPlayer();
	Vec3 playerPosition = player.GetPosition();

	player.Update(deltaTime);

	player.SetPosition(playerPosition);
	player.SetVelocity(Vec3(0.0f, 0.0f, 0.0f));

	if (game.CheckDebugPassword("record")) {
		// Return to GameStatePlay
		game.SetGameState<GameStatePlay>();
	}

	// Update sound recording
	FMOD::System* system = game.GetSound().GetSystem();
	if (game.GetInput().IsKeyBooped(SDLK_SPACE)) {
		if (!isRecording) {
			int numDrivers, numConnectedRecordDevices;
			system->getRecordNumDrivers(&numDrivers, &numConnectedRecordDevices);

			if (numConnectedRecordDevices > 0) {
				int nativeRate = 0;
				int nativeChannels = 0;
				const int recordDeviceIndex = 0;
				char deviceName[256];

				FMOD_RESULT result = system->getRecordDriverInfo(recordDeviceIndex, deviceName, sizeof (deviceName), NULL, &nativeRate, NULL, &nativeChannels, NULL);

				FMOD_CREATESOUNDEXINFO exinfo = {0};
				exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
				exinfo.numchannels      = nativeChannels;
				exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
				exinfo.defaultfrequency = nativeRate;
				exinfo.length           = nativeRate * sizeof(short) * nativeChannels * 10; // 10 secs of recording
				
				// Start recording
				result = system->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &recordedSound);
				result = system->recordStart(recordDeviceIndex, recordedSound, true);

				unsigned int soundLength = 0;
				result = recordedSound->getLength(&soundLength, FMOD_TIMEUNIT_PCM);
				soundLength = soundLength;

				numSwitchPositions = 0;
				isRecording = true;
				isPlaying = false;

				if (playingChannel) {
					playingChannel->stop();
					playingChannel = nullptr;
				}
			}
		} else {
			if (numSwitchPositions < 20) {
				unsigned int position;
				playingChannel->getPosition(&position, FMOD_TIMEUNIT_PCM);

				openPositions[numSwitchPositions++] = position;
			}

			isRecording = false;
			system->recordStop(0);

			isPlaying = true;
			currentSwitchPosition = -1;

			recordedSound->seekData(0);
			system->playSound(recordedSound, nullptr, true, &playingChannel);
		}
	}

	// Update mouth open/closed state
	float openness = game.GetPlayer().GetLaserPower();
	bool lastIsMouthOpen = isMouthOpen;

	if (isMouthOpen) {
		if (openness < upperOpenThreshold - thresholdRange) {
			isMouthOpen = false;
			lowerClosedThreshold = openness;
		}

		if (openness > upperOpenThreshold) {
			upperOpenThreshold = openness;
		}
	} else if (!isMouthOpen) {
		if (openness > lowerClosedThreshold + thresholdRange) {
			isMouthOpen = true;
			upperOpenThreshold = openness;

			if (isPlaying) {
				++currentSwitchPosition;
				
				if (currentSwitchPosition >= numSwitchPositions) {
					currentSwitchPosition = -1;
					playingChannel->setPosition(0, FMOD_TIMEUNIT_PCM);
				}
			}
		}

		if (openness < lowerClosedThreshold) {
			lowerClosedThreshold = openness;
		}
	}

	// Record word points
	if (isRecording && game.GetInput().GetMouseScroll()) {
		if (numSwitchPositions < 20) {
			unsigned int position;

			system->getRecordPosition(0, &position);

			openPositions[numSwitchPositions++] = position;
		}
	}

	// Playing responds to mouth movement
	if (playingChannel && currentSwitchPosition < numSwitchPositions && currentSwitchPosition != -1) {
		unsigned int position;
		bool isPaused;
		playingChannel->getPosition(&position, FMOD_TIMEUNIT_PCM);
		playingChannel->getPaused(&isPaused);

		if (position < openPositions[currentSwitchPosition]) {
			playingChannel->setPaused(false);
			
			if (currentSwitchPosition > 1 && position < openPositions[currentSwitchPosition - 1]) {
				playingChannel->setPosition(openPositions[currentSwitchPosition - 1], FMOD_TIMEUNIT_PCM);
			}
		} else if (position >= openPositions[currentSwitchPosition]) {
			playingChannel->setPosition(openPositions[currentSwitchPosition], FMOD_TIMEUNIT_PCM);
			playingChannel->setPaused(true);
		}
	}
}

void GameStateRecordSpeech::Render() {
	// Render the player at a good position
	game.GetCamera().SetPosition(game.GetPlayer().GetPosition() + Vec3(-200.0f, -200.0f, -3.0f));
	
	game.GetPlayer().Render();

	// Render sound marker box?

	// Render debug text
	RenderDebug();
}

void GameStateRecordSpeech::RenderDebug() {
	DebugStringBox* debug = game.GetDebugBox();

	if (debug) {
		FMOD::System* system = game.GetSound().GetSystem();
		unsigned int playbackPos = 0;

		if (recordedSound) {
			playingChannel->getPosition(&playbackPos, FMOD_TIMEUNIT_MS);
		}

		// Render debug info
		debug->DrawString(StaticString<80>::FromFormat("Open: %s", isMouthOpen ? "yes" : "no"));
		debug->DrawString(StaticString<80>::FromFormat("CurrentSoundPosition: %i", currentSwitchPosition));
		debug->DrawString(StaticString<80>::FromFormat("NumSwitchPositions: %i", numSwitchPositions));
		debug->DrawString(StaticString<80>::FromFormat("PlaybackPos: %i", playbackPos));

		// Render record progress
		if (recordedSound) {
			unsigned int recordPosition;
			int recordHz;

			system->getRecordPosition(0, &recordPosition);
			system->getRecordDriverInfo(0, nullptr, 0, nullptr, &recordHz, nullptr, nullptr, nullptr);

			debug->DrawString(StaticString<80>::FromFormat("Recording sound progress: %.2f sec", (float)recordPosition / (float)recordHz));
		}
	}
}

bool GameStateRecordSpeech::Enter() {
	FMOD::System* system = game.GetSound().GetSystem();

	if (!system) {
		return false;
	}

	return true;
}

void GameStateRecordSpeech::Exit() {
	if (recordedSound) {
		FMOD::System* system = game.GetSound().GetSystem();

		recordedSound->release();
		recordedSound = nullptr;
	}
}