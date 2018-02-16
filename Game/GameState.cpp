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
	game.RenderText(StaticString<80>::FromFormat("FPS: %i", 1.0f / game.GetDeltaTime()), 0, 0);
}

bool GameStatePlay::Enter() {
	return true;
}

void GameStatePlay::Exit() {
	return;
}