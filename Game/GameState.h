#pragma once

enum GameStateType {
	Play = 0,
	Editor = 1,
};

class GameState {
public:
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual bool Enter() = 0;
	virtual void Exit() = 0;

	virtual GameStateType GetType() const = 0;
};

class GameStatePlay : public GameState {
public:
	void Update(float deltaTime) override;
	void Render() override;

	bool Enter() override;
	void Exit() override;

	GameStateType GetType() const override {return Play;}
	//static GameStateType::GetType() const {return Play;}
};

class GameStateEditor : public GameState {
public:
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual bool Enter() = 0;
	virtual void Exit() = 0;

	GameStateType GetType() const override {return Editor;}
	//static GameStateType::GetType() const {return Play;}
};