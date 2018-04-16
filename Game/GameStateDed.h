#pragma once
#include "GameState.h"

class GameStateDed : public GameState {
public:
	void Update(float deltaTime) override;
	void Render() override;

	bool Enter() override;
	void Exit() override;

	GameStateType GetType() const override {return Ded;}

private:
	// Default time, in seconds, that it takes to respawn
	const float defaultRespawnTime = 4.0f;

	float respawnTimer = defaultRespawnTime;
};