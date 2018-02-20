#include "Game.h"
#include "GameState.h"
#include "GameStateEditor.h"
#include "FileDialog.h"

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