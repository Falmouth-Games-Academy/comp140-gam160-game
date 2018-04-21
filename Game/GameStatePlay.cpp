#include "Game.h"
#include "GameStatePlay.h"
#include "GameStateEditor.h"
#include "GameStateRecordSpeech.h"

#include <SDL.h>

void GameStatePlay::Update(float deltaTime) {
	// Update gesture input
	game.GetGesture().Update();

	// Update objects (including the player)
	/*for (Object* obj : game.GetObjects()) {
		obj->Update(deltaTime);
	}*/
	// Changed because the iterator changes if the list is reallocated. Todo: Is there a more elegant solution?
	Array<Object*>& objects = game.GetObjects();
	for (int i = 0; i < objects.GetNum(); ++i) {
		objects[i]->Update(deltaTime);
	}

	// Update camera
	game.GetCamera().Update(deltaTime);

	// Switch game states if the editor button was pressed
	if (game.CheckDebugPassword("edit")) {
		game.SetGameState<GameStateEditor>();
	}

	if (game.CheckDebugPassword("record")) {
		game.SetGameState<GameStateRecordSpeech>();
	}
}

void GameStatePlay::Render() {
	// Render the level first
	game.GetLevel().Render();

	// Render gesture debug stuff
	game.GetGesture().Render();

	// Render objects (including the player)
	for (Object* object : game.GetObjects()) {
		object->Render();
	}

	// Render camera (layered and debug stuff)
	game.GetCamera().Render();

	// Render HUD
	game.GetHud().Render();

	// Render debug text
	if (DebugStringBox* debug = game.GetDebugBox()) {
		debug->DrawString(StaticString<80>::FromFormat("FPS: %f", 1.0f / game.GetDeltaTime()));
		debug->DrawString(StaticString<80>::FromFormat("NumCachedImages: %i Refs: %i NumObjects: %i", 
			game.GetImageCache().GetNum(), game.GetImageCache().GetNumReferences(), game.GetObjects().GetNum()));
	}
}

bool GameStatePlay::Enter() {
	game.GetCamera().SetFollowingPlayer(true);
	return true;
}

void GameStatePlay::Exit() {
	game.GetCamera().SetFollowingPlayer(false);
	return;
}