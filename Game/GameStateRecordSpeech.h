#pragma once
#include "GameState.h"

namespace FMOD {class Sound;};

class GameStateRecordSpeech : public GameState {
public:
	void Update(float deltaTime) override;
	void Render() override;

	bool Enter() override;
	void Exit() override;

	GameStateType GetType() const override {return RecordSpeech;}

	// Debug render
	void RenderDebug();

private:
	// The sound data being recorded to, or nullptr if nothing is being recorded
	FMOD::Sound* recordedSound;
};