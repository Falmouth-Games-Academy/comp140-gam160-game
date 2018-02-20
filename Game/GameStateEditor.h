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
	void UpdateMoveables();

	// Cursor update functions
	void UpdateCursorDrawingLayer();
	void UpdateCursorPositioningLayer();

	GameStateType GetType() const override {return Editor;}
	//static GameStateType::GetType() const {return Play;}

private:
	enum CursorState : uint8 {
		Normal = 0,
		PositioningLayer,
		DrawingLayer,
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