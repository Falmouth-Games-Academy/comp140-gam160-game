#include "Game.h"
#include "GameState.h"

#include <SDL.h>

void GameStatePlay::Update(float deltaTime) {
	// Update gesture input
	game.GetGesture().Update();

	// Update player
	game.GetPlayer().Update(deltaTime);
	
	// Update camera
	game.GetCamera().Update();

	// Switch game states if the editor button was pressed
	if (game.GetInput().IsKeyBooped(SDLK_e)) {
		game.SetGameState<GameStateEditor>();
	}
}

void GameStatePlay::Render() {
	// Render the level first
	game.GetLevel().Render();

	// Render gesture debug stuff
	game.GetGesture().Render();

	// Render player
	game.GetPlayer().Render();

	// Render camera (debug stuff)
	game.GetCamera().Render();

	// Render text
	game.RenderText(StaticString<80>::FromFormat("FPS: %f", 1.0f / game.GetDeltaTime()), 0, 0);
}

bool GameStatePlay::Enter() {
	game.GetCamera().SetFollowingPlayer(true);
	return true;
}

void GameStatePlay::Exit() {
	game.GetCamera().SetFollowingPlayer(false);
	return;
}

void GameStateEditor::Update(float deltaTime) {
	// Update camera
	game.GetCamera().Update();

	// Perform editor updates
	UpdateCursor();
	UpdateCameraControls();
	UpdateSelections();
	UpdateMoveables();

	// Switch back to gameplay if the editor button was pressed
	if (game.GetInput().IsKeyBooped(SDLK_e)) {
		game.SetGameState<GameStatePlay>();
	}
}

void GameStateEditor::Render() {
	// Render the level
	game.GetLevel().Render();

	// Render the objects
	// There are no objects lawl

	// Render player
	game.GetPlayer().Render();

	// Render outlines around selected items
	const Array<BackgroundLayer>& levelLayers = game.GetLevel().GetLayers();
	for (int32 index : selectedItems[SelectionType::BgLayer]) {
		if (levelLayers.IsIndexValid(index)) {
			game.GetCamera().RenderRectangle(levelLayers[index].GetPosition(), levelLayers[index].GetSize(), clrRed);
		}
	}

	// Render selection border
	//Vec3 start3D = game.GetCamera().ScreenToWorld(selectStartPosition), end3D = game.GetCamera().ScreenToWorld(cursorScreenPosition);
	//game.GetCamera().RenderRectangle(start3D, (end3D - start3D).xy, clrRed);

	// Draw debug information
	DebugStringBox box(RenderScreen::Main, 0, 0, 200, 300);
	Vec3 cameraPosition = game.GetCamera().GetPosition();

	box.DrawString("Basic controls: LeftClick: Select, ScrollWheel: Camera zoom, RightClick: Pan camera, Z: Reset zoom, C: Centre camera to player");
	box.DrawString("Object controls: LeftClick: Select, ScrollWheel: RightClick: Pan camera");
	box.DrawString("");
	box.DrawString(StaticString<140>::FromFormat("Camera position: %.2f,%.2f,%.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z));
	box.DrawString(StaticString<140>::FromFormat("Cursor position: %.2f,%.2f,%.2f", cursorPosition.x, cursorPosition.y, cursorPosition.z));
	box.DrawString(StaticString<140>::FromFormat("Select position: %.2f,%.2f to ", selectStartPosition.x, selectStartPosition.y, cursorScreenPosition.x, cursorScreenPosition.y));
}

bool GameStateEditor::Enter() {
	return true;
}

void GameStateEditor::Exit() {
}

void GameStateEditor::UpdateCursor() {
	// Update cursor position, done!
	lastCursorScreenPosition = cursorScreenPosition;
	
	cursorScreenPosition = Vec3(game.GetInput().GetMousePosition(), cursorPosition.z);
	cursorPosition = game.GetCamera().ScreenToWorld(cursorScreenPosition);
}

void GameStateEditor::UpdateCameraControls() {
	Camera& camera = game.GetCamera();

	// Pan camera
	if (game.GetInput().IsMouseDown(InputManager::RightButton)) {
		camera.GoToPosition(camera.GetTargetPosition() + (camera.ScreenToWorld(lastCursorScreenPosition) - camera.ScreenToWorld(cursorScreenPosition)));
	}

	// Zoom camera
	int mouseScroll = game.GetInput().GetMouseScroll();

	if (mouseScroll != 0) {
		camera.GoToPosition(camera.GetTargetPosition() + Vec3(0.0f, 0.0f, (float)mouseScroll));
	}
}

void GameStateEditor::UpdateSelections() {
	const float singleSelectionRadius = 5.0f;
	bool isMouseSingleSelecting = game.GetInput().IsMouseUnbooped(InputManager::LeftButton) && Vec2::Distance(selectStartPosition.xy, cursorScreenPosition.xy) <= singleSelectionRadius;
	bool isMouseRangeSelecting = false;

	if (game.GetInput().IsMouseBooped(InputManager::LeftButton)) {
		selectStartPosition = cursorScreenPosition;
	}

	if (isMouseSingleSelecting) {
		// Preserve the last selected item for layering tests
		int lastLayerDepth = 0.0f;
		if (selectedItems[selectionType].GetNum() == 1) {
			// Holy cow these lines of code are a mouthful.
			if (game.GetLevel().GetLayers().IsIndexValid(selectedItems[selectionType][0])) {
				lastLayerDepth = game.GetLevel().GetLayers()[selectedItems[selectionType][0]].GetPosition().z;
			}
		}

		// Clear the selections list
		selectedItems[selectionType].Clear();

		switch (selectionType) {
			case BgLayer: {
				if (BackgroundLayer* layer = game.GetLevel().GetLayerAtScreenPosition(cursorScreenPosition.xy)) {
					selectedItems[BgLayer].Set({int32(layer->GetIndex())});

					cursorPosition.z = layer->GetPosition().z;
				}
			}
		}
	}
}

void GameStateEditor::UpdateMoveables() {
	if (game.GetInput().IsMouseDown(InputManager::LeftButton) && selectedItems[selectionType].GetNum() >= 0) {
		Vec2 cursorScreenDelta = cursorScreenPosition.xy - lastCursorScreenPosition.xy;
		float cameraZ = game.GetCamera().GetPosition().z;

		switch (selectionType) {
			case BgLayer: {
				Array<BackgroundLayer>& layers = game.GetLevel().GetLayers();
				for (int32 index : selectedItems[BgLayer]) {
					if (layers.IsIndexValid(index)) {
						layers[index].SetPosition(layers[index].GetPosition() + cursorScreenDelta * (layers[index].GetPosition().z - cameraZ));
					}
				}
			}
		}
	}
}
