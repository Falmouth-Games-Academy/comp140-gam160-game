#pragma once
#include "GameState.h"

class GameStatePlay : public GameState {
public:
	void Update(float deltaTime) override;
	void Render() override;

	bool Enter() override;
	void Exit() override;

	GameStateType GetType() const override {return Play;}
	//static GameStateType::GetType() const {return Play;}
};