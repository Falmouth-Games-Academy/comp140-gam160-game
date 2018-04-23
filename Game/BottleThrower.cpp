#include "BottleThrower.h"
#include "Bottle.h"
#include "Game.h"

void BottleThrower::OnSpawn() {
	updateFlags = 0;

	editorLabel = "BottleThrow";

	bottleSpawnTimer = bottleSpawnRate;
}

void BottleThrower::Update(float deltaTime) {
	// Check if a bottle can be spawned yet
	if (bottleSpawnTimer > 0.0f) {
		bottleSpawnTimer -= deltaTime;

		if (bottleSpawnTimer <= 0.0f) {
			// Spawn a bottle!
			Bottle* bottle = game.SpawnObject<Bottle>();

			bottle->SetPosition(Vec3(position.xy, game.GetPlayer().GetPosition().z));

			// Toss it in a random upward direction
			const float tossSpeed = 5000.0f;
			bottle->Toss(Vec3(Math::randfloat(-1.0f, 1.0f), Math::randfloat(-1.0f, -0.3f), 0.0f).Normalised() * tossSpeed);
			
			// Reset the timer
			bottleSpawnTimer = bottleSpawnRate;
		}
	}
}