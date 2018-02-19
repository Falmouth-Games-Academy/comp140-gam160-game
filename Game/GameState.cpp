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

	// Update custom camera panning
	UpdateCameraControls();

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

	// Draw debug information
	DebugStringBox box(Game::Main, 0, 0, 200, 300);

	box.DrawString("Basic controls: LeftClick: Select, ScrollWheel: Camera zoom, RightClick: Pan camera, Z: Reset zoom, C: Centre camera to player");
	box.DrawString("Object controls: LeftClick: Select, ScrollWheel: RightClick: Pan camera");
	box.DrawString("");
	box.DrawString(StaticString<80>::FromFormat("Cursor position: %.2f,%.2f,%.2f", cursorPosition.x, cursorPosition.y, cursorPosition.z));
}

bool GameStateEditor::Enter() {
	return true;
}

void GameStateEditor::Exit() {
}

void GameStateEditor::UpdateCameraControls() {
	Camera& camera = game.GetCamera();
	Vec3 nextCursorPosition(game.GetInput().GetMousePosition(), cursorPosition.z);

	nextCursorPosition.z = 1.0f;
	// Pan camera
	if (game.GetInput().IsMouseDown(InputManager::RightButton)) {
		camera.SetPosition(camera.GetPosition() + (camera.ScreenToWorld(cursorPosition) - camera.ScreenToWorld(nextCursorPosition)));
	}

	// Zoom camera
	int mouseScroll = game.GetInput().GetMouseScroll();

	if (mouseScroll != 0) {
		camera.SetPosition(camera.GetPosition() + Vec3(0.0f, 0.0f, (float)mouseScroll));
	}

	// Update cursor position, done!
	cursorPosition = nextCursorPosition;
}