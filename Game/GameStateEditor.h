#pragma once
#include "GameState.h"
#include "Containers.h"

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
	void UpdateCursorDraggingLayer();

	// Cursor update functions
	void UpdateCursorDrawingLayer();
	void UpdateCursorPlacingLayer();
	void UpdateCursorDrawingCollision();

	GameStateType GetType() const override {return Editor;}
	//static GameStateType::GetType() const {return Play;}

private:
	enum CursorState : uint8 {
		Normal = 0,
		DraggingLayer,
		PlacingLayer,
		DrawingLayer,
		DrawingCollision,
		NumCursorStates,
	};

	// Debug info
	class DebugStringBox* debug;

	// Current cursor state
	CursorState cursorState;

	// Current position of the mouse cursor, in level units
	Vec3 cursorPosition;

	// Current position of the mouse cursor, in screen units
	Vec3 cursorScreenPosition;
	Vec3 lastCursorScreenPosition;

	// Position of the mouse cursor at the beginning of a selection
	Vec3 selectStartPosition;

	// CursorState::CreatingLayer
	int32 cursorCreatingLayerIndex;

	// CursorState::DrawingCollision
	Vec3 cursorStartCollisionPosition;

	// Cursor graphics for feedback
	SDL_Cursor* cursorSprites[NumCursorStates];

	// Selection list
	Array<class Object*> selectedItems;
};