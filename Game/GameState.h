#pragma once
#include "Math.h" // Vec3

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
	void Update(float deltaTime) override;
	void Render() override;

	bool Enter() override;
	void Exit() override;

	// Update functions
	void UpdateCursor();
	void UpdateCameraControls();
	void UpdateSelections();
	void UpdateMoveables();

	GameStateType GetType() const override {return Editor;}
	//static GameStateType::GetType() const {return Play;}

private:
	// Current position of the mouse cursor, in level units
	Vec3 cursorPosition;

	// Current position of the mouse cursor, in screen units
	Vec3 cursorScreenPosition;
	Vec3 lastCursorScreenPosition;

	// Position of the mouse cursor at the beginning of a selection
	Vec3 selectStartPosition;

	// Selections
	enum SelectionType {
		BgLayer = 0,
		Object = 1,
		NumSelectionTypes,
	};

	SelectionType selectionType;

	Array<int32> highlightedItems[NumSelectionTypes];
	Array<int32> selectedItems[NumSelectionTypes];
};