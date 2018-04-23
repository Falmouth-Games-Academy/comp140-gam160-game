#pragma once

enum GameStateType {
	Play = 0,
	Editor = 1,
	RecordSpeech = 2,
	Ded = 3,
	Calibrate = 4,
};

class GameState {
public:
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual bool Enter() = 0;
	virtual void Exit() = 0;

	virtual GameStateType GetType() const = 0;
};