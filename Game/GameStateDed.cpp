#include "GameStateDed.h"
#include "GameStatePlay.h"
#include "Game.h"

void GameStateDed::Update(float deltaTime) {
	// Make Handzer look dead
	game.GetPlayer().GetSprite().SetBlendColour(Colour(0, 0, 0, 255));

	// Count down the respawn timer
	respawnTimer -= deltaTime;

	if (respawnTimer <= 0.0f) {
		// Respawn the player
		game.RespawnPlayer();

		game.SetGameState<GameStatePlay>();
	}
}

void GameStateDed::Render() {
	// Render the player
	game.GetPlayer().Render();

	// Render the camera
	game.GetCamera().Render();

	// Render Ded text after a second
	if (respawnTimer <= defaultRespawnTime - 1.0f) {
		Dimensions2 screenSize = game.GetRenderer().GetScreenSize();

		game.GetRenderer().RenderText("Ded.", screenSize.width / 2, screenSize.height / 2, Colour::Red(), true, true);
	}
}

bool GameStateDed::Enter() {
	respawnTimer = defaultRespawnTime;

	return true;
}

void GameStateDed::Exit() {
	return;
}