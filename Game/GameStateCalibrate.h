#pragma once
#include "GameState.h"
#include "Sprite.h"

class GameStateCalibrate : public GameState {
public:
	void Update(float deltaTime) override;
	void Render() override;

	bool Enter() override;
	void Exit() override;

	GameStateType GetType() const override {return Calibrate;}

private:
	// State of the calibration process
	int32 state = 0;

	// Image of the controller setup
	Sprite handzerSetup;

	// Images of handzer in his mouth open and closed states
	Sprite handzerClosed;
	Sprite handzerOpen;

	const int numStates = 3;
};