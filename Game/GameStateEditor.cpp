#include "Game.h"
#include "GameStatePlay.h"
#include "GameStateEditor.h"
#include "FileDialog.h"
#include "Object.h"
#include "DataFile.h"

void GameStateEditor::Update(float deltaTime) {
	// Update camera
	game.GetCamera().Update(deltaTime);

	// Perform editor updates
	UpdateCursor();
	UpdateCameraControls();
	UpdateSelections();

	// Switch back to gameplay if the editor button was pressed
	if (game.GetInput().IsKeyBooped(SDLK_e)) {
		game.SetGameState<GameStatePlay>();
	}
}

void GameStateEditor::Render() {
	// Render the level
	game.GetLevel().Render();

	// Render the objects
	for (Object* obj : game.GetObjects()) {
		obj->Render();
	}

	// Render camera
	game.GetCamera().Render();

	// Render outlines around selected items
	for (Object* obj : selectedItems) {
		game.GetCamera().RenderRectangle(obj->GetPosition() - obj->GetSprite().GetOrigin(), obj->GetSize(), Colour::Red());
	}

	// Render debug appurtenances
	game.RenderDebugAppurtenances();

	// Render selection border
	//Vec3 start3D = game.GetCamera().ScreenToWorld(selectStartPosition), end3D = game.GetCamera().ScreenToWorld(cursorScreenPosition);
	//game.GetCamera().RenderRectangle(start3D, (end3D - start3D).xy, clrRed);

	// Draw debug information
	DebugStringBox* debug = game.GetDebug();
	Vec3 cameraPosition = game.GetCamera().GetPosition();

	if (debug) {
		debug->DrawString(StaticString<140>::FromFormat("Cursor position: %.2f,%.2f,%.2f", cursorPosition.x, cursorPosition.y, cursorPosition.z));
		debug->DrawString(StaticString<140>::FromFormat("Select position: %.2f,%.2f", selectStartPosition.x, selectStartPosition.y, cursorScreenPosition.x, cursorScreenPosition.y));
		debug->DrawString("LClick: Select/Drag  Space+LClick: Move away/closer S+LClick: Scale");
		debug->DrawString("ScrollWheel: Zoom, RightClick: Pan, Z: Reset zoom, C: Centre camera on player");
		debug->DrawString("L: Create background layer, O: Create object");
	}

	// Render the depth view box
	if (selectedItems.GetNum()) {
		RenderDepthView();
	}
}

void GameStateEditor::RenderDepthView() {
	const Vec3 depthRectSize(100.0f, 100.0f, 20.0f);

	// Render the depth screen background
	SDL_Renderer* renderer = game.GetRenderer();
	SDL_Rect depthRect = {0, 0, (int)depthRectSize.x, (int)depthRectSize.y};

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &depthRect);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	// Render objects and background layers in the depth screen
	MinMax<float> depthXRange(game.GetCamera().ScreenToWorld(Vec3(0.0f, 0.0f, depthRectSize.z)).x, 
		game.GetCamera().ScreenToWorld(Vec3(game.GetCamera().GetViewSize().x, 0.0f, depthRectSize.z)).x);

	// Render background layers
	for (BackgroundLayer& layer : game.GetLevel().GetLayers()) {
		// Check the layer is in view
		if (layer.GetPosition().x + layer.GetSprite().GetDimensions().x >= depthXRange.min && 
			layer.GetPosition().x <= depthXRange.max) {

			// Scale the layer into the depthRect for a top-down view
			Vec2 lineStart((layer.GetPosition().x - depthXRange.min) * depthRectSize.x / depthXRange.GetRange(), 
				depthRectSize.y - layer.GetPosition().z * depthRectSize.y / depthRectSize.z);
			Vec2 lineEnd = lineStart + Vec2(layer.GetSprite().GetDimensions().x * depthRectSize.x / depthXRange.GetRange(), 0.0f);

			// Render
			SDL_RenderDrawLine(renderer, (int)lineStart.x, (int)lineStart.y, (int)lineEnd.x, (int)lineEnd.y);
		}
	}
}

bool GameStateEditor::Enter() {
	// Create cursors
	memset(cursorSprites, 0, sizeof (cursorSprites));

	cursorSprites[Normal] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	cursorSprites[PlacingLayer] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);

	return true;
}

void GameStateEditor::Exit() {
	for (int i = 0; i < NumCursorStates; ++i) {
		if (cursorSprites[i]) {
			SDL_FreeCursor(cursorSprites[i]);
		}
	}
}

void GameStateEditor::UpdateCursor() {
	// Update cursor position!
	lastCursorScreenPosition = cursorScreenPosition;
	
	cursorScreenPosition = Vec3(game.GetInput().GetMousePosition(), cursorPosition.z);
	cursorPosition = game.GetCamera().ScreenToWorld(cursorScreenPosition);

	// Perform per-state cursor updates
	SDL_Cursor* cursorOverride = nullptr;

	switch (cursorState) {
		case Normal:
			UpdateCursorNormal();
			break;
		case DraggingLayer:
			UpdateCursorDraggingLayer();
			break;
		case PlacingLayer:
			UpdateCursorPlacingLayer();
			break;
		case DrawingLayer:
			UpdateCursorDrawingLayer();
			break;
		case DrawingCollision:
			UpdateCursorDrawingCollision();
			break;
		case PlacingObject:
			UpdateCursorPlacingObject();
			break;
	}

	// Show cursor feedback to user
	if (cursorOverride) {
		SDL_SetCursor(cursorOverride);
	} else if (cursorSprites[cursorState]) {
		SDL_SetCursor(cursorSprites[cursorState]);
	} else {
		SDL_SetCursor(cursorSprites[Normal]); // Todo default
	}
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

void GameStateEditor::UpdateCommands() {
	const char* layerCreationFilename = game.GetInput().GetDroppedFile();

	// If Ctrl key is pressed
	if (game.GetInput().IsKeyDown(SDLK_LCTRL)) {
		// Ctrl+L: Create new layer
		if (game.GetInput().IsKeyBooped(SDLK_l)) {
			// Open a dialogue to load the layer
			FileDialog dialog(FileDialog::OpenFile, "Image files/*.JPG;*.PNG;*.BMP");

			dialog.Open();

			// Keep the filename
			layerCreationFilename = dialog.GetResult();
		}
		// Ctrl+S: Save map
		else if (game.GetInput().IsKeyBooped(SDLK_s)) {
			// Prompt the user to save the level file somewhere
			FileDialog dialog(FileDialog::SaveFile, "Handzer Data File/*.hdf");

			dialog.Open();

			// Ensure the user choses a file name
			if (const char* filename = dialog.GetResult()) {
				game.GetLevel().Save(filename);
			}
		}
		// Ctrl+O: Open map
		else if (game.GetInput().IsKeyBooped(SDLK_o)) {
			FileDialog dialog(FileDialog::OpenFile, "Handzer Data File/*.hdf");

			dialog.Open();

			if (const char* filename = dialog.GetResult()) {
				game.GetLevel().Load(filename);
			}
		}
	}

	// Create a new background layer if a layer was dropped or chosen via Ctrl+L
	if (layerCreationFilename) {
		cursorCreatingLayerPtr = game.GetLevel().CreateLayer(game.GetInput().GetDroppedFile(), cursorPosition);

		// Update the cursor state
		cursorState = PlacingLayer;
	}

	// DEL: Delete selected layer(s)
	if (game.GetInput().IsKeyBooped(SDLK_DELETE)) {
		for (int i = 0; i < selectedItems.GetNum(); ++i) {
			if (selectedItems[i]->GetType() == Object::BackgroundLayerType) {
				game.GetLevel().DestroyLayer((BackgroundLayer*)selectedItems[i]);
			} else {
				selectedItems[i]->Destroy(); // Todo: objects should use one or the other function
			}
		}
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
		// Clear the selections list
		selectedItems.Clear();

		// Find a layer underneath the mouse and select it
		if (BackgroundLayer* layer = game.GetLevel().GetLayerAtScreenPosition(cursorScreenPosition.xy)) {
			selectedItems.Set({ layer });

			cursorPosition.z = layer->GetPosition().z;
		}

		// Look for objects to select
		for (Object* obj : game.GetObjects()) {
			if (game.GetCamera().WorldToScreen(obj->GetPosition() - obj->GetSprite().GetOrigin()).xy <= cursorScreenPosition.xy && 
					game.GetCamera().WorldToScreen(obj->GetPosition() - obj->GetSprite().GetOrigin() + obj->GetSprite().GetDimensions()).xy >= cursorScreenPosition.xy) {
				// Select this object, if it's the closest or only one under the mouse
				if (selectedItems.GetNum() == 0 || (selectedItems[0] && selectedItems[0]->GetPosition().z > obj->GetPosition().z)) {
					selectedItems.Set({ obj });
				}
			}
		}
	}
}

void GameStateEditor::UpdateCursorNormal() {
	// Update selections (clicks)
	UpdateSelections();

	// Update commands (ctrl+cmds, file drops, etc)
	UpdateCommands();

	// O: Create object
	if (!game.GetInput().IsKeyDown(SDLK_LCTRL)) {
		if (game.GetInput().IsKeyBooped(SDLK_o)) {
			cursorState = PlacingObject;
		}

		// C+Drag: Draw collision box onto selected layer
		else if (game.GetInput().IsKeyDown(SDLK_c) && selectedItems.GetNum() == 1) {
			cursorState = DrawingCollision;
		}
	}

	// LButton alone: Drag selected layer(s)
	if (cursorState == Normal && game.GetInput().IsMouseDown(InputManager::LeftButton)) {
		cursorState = DraggingLayer;
	}
}

void GameStateEditor::UpdateCursorDraggingLayer() {
	if (game.GetInput().IsMouseDown(InputManager::LeftButton) && selectedItems.GetNum() >= 0) {
		// Move layers
		Vec2 cursorScreenDelta = cursorScreenPosition.xy - lastCursorScreenPosition.xy;
		float cameraZ = game.GetCamera().GetPosition().z;
		enum DragType {Move, Depth, Scale};
		DragType dragType = (game.GetInput().IsKeyDown(SDLK_SPACE) ? Depth : (game.GetInput().IsKeyDown(SDLK_s) ? Scale : Move));

		for (Object* obj : selectedItems) {
			switch (dragType) {
				case Move:
					// Drag to the sides
					obj->SetPosition(obj->GetPosition() + cursorScreenDelta * (obj->GetPosition().z - cameraZ));
					break;
				case Depth:
					// Drag toward/away from camera
					obj->SetPosition(Vec3(obj->GetPosition().xy, obj->GetPosition().z - cursorScreenDelta.y * 0.005f));
					break;
				case Scale:
					// Scale up/down
					obj->SetSize(obj->GetSize() * (1.0f + cursorScreenDelta.y * 0.01f));
					break;
			}
		}
	} else {
		cursorState = Normal;
	}
}

void GameStateEditor::UpdateCursorDrawingLayer() {
	// Obtain the layer being created
	if (cursorCreatingLayerPtr) {
		Vec2 size = cursorPosition.xy - cursorCreatingLayerPtr->GetPosition().xy;
		
		size.y = size.x * cursorCreatingLayerPtr->GetSprite().GetBaseDimensions().y / cursorCreatingLayerPtr->GetSprite().GetBaseDimensions().x;
		cursorCreatingLayerPtr->SetSize(size);
	}

	// Update cursor state
	if (game.GetInput().IsMouseBooped(InputManager::LeftButton)) {
		cursorState = Normal;
	}
}

void GameStateEditor::UpdateCursorPlacingLayer() {
	BackgroundLayer* layer = nullptr;

	// Obtain the layer being created
	if (cursorCreatingLayerPtr) {
		cursorCreatingLayerPtr->SetPosition(cursorPosition);
	}

	// Update cursor state upon release
	if (game.GetInput().IsMouseBooped(InputManager::LeftButton)) {
		cursorState = DrawingLayer;
	}
}

void GameStateEditor::UpdateCursorDrawingCollision() {
	// Set the initial cursor position
	if (game.GetInput().IsMouseBooped(InputManager::LeftButton)) {
		cursorStartCollisionPosition = cursorPosition;
	}

	// Check if mouse is down
	if (game.GetInput().IsMouseDown(InputManager::LeftButton)) {
		if (selectedItems.GetNum() == 1) {
			selectedItems[0]->SetCollision(&Rect2((cursorStartCollisionPosition.xy - selectedItems[0]->GetPosition().xy) / selectedItems[0]->GetSprite().GetScale(), 
												  ((cursorPosition - cursorStartCollisionPosition).xy) / selectedItems[0]->GetSprite().GetScale()));
		}
	}

	// Return back to normal cursor state if necessary
	if (!game.GetInput().IsKeyDown(SDLK_c)) {
		cursorState = Normal;
	}
}

void GameStateEditor::UpdateCursorPlacingObject() {
	// Create object if it doesn't exist
	if (!cursorCreatingObjectPtr) {
		cursorCreatingObjectPtr = game.SpawnObject(cursorCreatingObjectType);
	}

	// Reposition object
	if (cursorCreatingObjectPtr) {
		cursorCreatingObjectPtr->SetPosition(cursorPosition);
	}

	// Arrow keys: Change object type
	if (game.GetInput().IsKeyBooped(SDLK_LEFT) && cursorCreatingObjectType > (Object::Type)0) {
		if (cursorCreatingObjectPtr) {
			cursorCreatingObjectPtr->Destroy();
		}

		cursorCreatingObjectType = (Object::Type)((int)cursorCreatingObjectType - 1);
		cursorCreatingObjectPtr = game.SpawnObject(cursorCreatingObjectType);
	} else if (game.GetInput().IsKeyBooped(SDLK_RIGHT) && cursorCreatingObjectType < Object::NumTypes - 1) {
		if (cursorCreatingObjectPtr) {
			cursorCreatingObjectPtr->Destroy();
		}

		cursorCreatingObjectType = (Object::Type)((int)cursorCreatingObjectType + 1);
		cursorCreatingObjectPtr = game.SpawnObject(cursorCreatingObjectType);
	}

	// Return back to normal cursor state when object is placed
	if (game.GetInput().IsMouseBooped(InputManager::LeftButton)) {
		cursorCreatingObjectPtr = nullptr;
		cursorState = Normal;
	}
}
