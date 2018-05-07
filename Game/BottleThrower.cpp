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
			// Count the number of bottles first
			int numBottles = 0;
			for (Object* obj : game.GetObjects()) {
				if (obj->GetType() == Object::BottleType) {
					++numBottles;
				}
			}

			if (numBottles < maxNumBottles) {
				// Spawn a bottle!
				Bottle* bottle = game.SpawnObject<Bottle>();

				bottle->SetPosition(Vec3(position.xy, game.GetPlayer().GetPosition().z));

				// Toss it in a random upward direction
				const float tossSpeed = 7000.0f;
				bottle->Toss(Vec3(Math::randfloat(-1.0f, 1.0f), Math::randfloat(-2.0f, -0.5f), 0.0f) * tossSpeed);
			}
			
			// Reset the timer
			bottleSpawnTimer = bottleSpawnRate;
		}
	}
}