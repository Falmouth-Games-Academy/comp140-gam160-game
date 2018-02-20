#include "Game.h"
#include "GameStateEditor.h"
#include "FileDialog.h"

void GameStateEditor::Update(float deltaTime) {
	// Reset debug text box for rendering5
	debug->Reset();

	// Update camera
	game.GetCamera().Update();

	// Perform editor updates
	UpdateCursor();
	UpdateCameraControls();
	UpdateSelections();
	UpdateMoveables();

	if (game.GetInput().IsKeyBooped(SDLK_n)) {
		FileDialog dialog(FileDialog::OpenFile, "Image files/*.JPG;*.PNG;*.BMP");

		dialog.Open();

		// Try to load the new layer
		if (const char* filename = dialog.GetResult()) {
			auto& layers = game.GetLevel().GetLayers();

			layers.Append(layers.GetNum(), filename, cursorPosition, Vec2(1.0f, 1.0f));

			cursorCreatingLayerIndex = layers.GetNum() - 1;

			// Update the cursor state during scene layer build
			cursorState = PositioningLayer;
		}
	}

	// Perform cursor updates
	switch (cursorState) {
		case Normal:
			break;
		case PositioningLayer:
			UpdateCursorPositioningLayer();
			break;
		case DrawingLayer:
			UpdateCursorDrawingLayer();
			break;
	}

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
	Vec3 cameraPosition = game.GetCamera().GetPosition();

	debug->DrawString("Basic controls: LeftClick: Select, ScrollWheel: Camera zoom, RightClick: Pan camera, Z: Reset zoom, C: Centre camera to player");
	debug->DrawString("Object controls: LeftClick: Select, ScrollWheel: RightClick: Pan camera");
	debug->DrawString("");
	debug->DrawString(StaticString<140>::FromFormat("Camera position: %.2f,%.2f,%.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z));
	debug->DrawString(StaticString<140>::FromFormat("Cursor position: %.2f,%.2f,%.2f", cursorPosition.x, cursorPosition.y, cursorPosition.z));
	debug->DrawString(StaticString<140>::FromFormat("Select position: %.2f,%.2f", selectStartPosition.x, selectStartPosition.y, cursorScreenPosition.x, cursorScreenPosition.y));
}

bool GameStateEditor::Enter() {
	debug = new DebugStringBox(RenderScreen::Main, 0, 0, 100, 100);
	return true;
}

void GameStateEditor::Exit() {
	delete debug;
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

void GameStateEditor::UpdateCursorDrawingLayer() {
	BackgroundLayer* layer = &game.GetLevel().GetLayers()[cursorCreatingLayerIndex];

	// Obtain the layer being created
	if (game.GetLevel().GetLayers().IsIndexValid(cursorCreatingLayerIndex)) {
		Vec2 size = cursorPosition.xy - layer->GetPosition().xy;
		
		size.y = size.x * layer->GetSprite().GetBaseDimensions().y / layer->GetSprite().GetBaseDimensions().x;
		layer->SetSize(size);
	}

	if (game.GetInput().IsMouseBooped(InputManager::LeftButton)) {
		cursorState = Normal;
	}
}

void GameStateEditor::UpdateCursorPositioningLayer() {
	BackgroundLayer* layer = nullptr;

	// Obtain the layer being created
	if (game.GetLevel().GetLayers().IsIndexValid(cursorCreatingLayerIndex)) {
		game.GetLevel().GetLayers()[cursorCreatingLayerIndex].SetPosition(cursorPosition);
	}

	// Update cursor state upon release
	if (game.GetInput().IsMouseBooped(InputManager::LeftButton)) {
		cursorState = DrawingLayer;
	}
}
