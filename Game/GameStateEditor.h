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
	void UpdateCommands();
	void UpdateSelections();

	// Cursor update functions
	void UpdateCursorNormal();
	void UpdateCursorDraggingLayer();
	void UpdateCursorDrawingLayer();
	void UpdateCursorPlacingLayer();
	void UpdateCursorDrawingCollision();
	void UpdateCursorPlacingObject();

	// Additional render functions
	void RenderDepthView();

	GameStateType GetType() const override {return Editor;}
	//static GameStateType::GetType() const {return Play;}

private:
	enum CursorState : uint8 {
		Normal = 0,
		DraggingLayer,
		PlacingLayer,
		DrawingLayer,
		DrawingCollision,
		PlacingObject,
		NumCursorStates,
	};

	// Current cursor state
	CursorState cursorState = Normal;

	// Current position of the mouse cursor, in level units
	Vec3 cursorPosition;

	// Current position of the mouse cursor, in screen units
	Vec3 cursorScreenPosition;
	Vec3 lastCursorScreenPosition;

	// Position of the mouse cursor at the beginning of a selection
	Vec3 selectStartPosition;

	// CursorState::CreatingLayer
	Object* cursorCreatingLayerPtr = nullptr;

	// CursorState::DrawingCollision
	Vec3 cursorStartCollisionPosition;

	// CursorState::PlacingObject
	Object::Type cursorCreatingObjectType = Object::BottleType;

	Object* cursorCreatingObjectPtr = nullptr;

	// Cursor graphics for feedback
	SDL_Cursor* cursorSprites[NumCursorStates];

	// Selection list
	Array<class Object*> selectedItems;
};