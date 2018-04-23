#pragma once
#include "Object.h"

class BottleThrower : public Object {
public:
	void OnSpawn() override;

	void Update(float deltaTime) override;

	Object::Type GetType() const override { return BottleThrowerType; }

public:
	// Maximum number of bottles allowed to live in the world before the spawner stops
	int32 maxNumBottles = 10;

	// Seconds per bottle spawn
	float32 bottleSpawnRate = 1.0f;

	// Current countdown timer for bottle spawns
	float32 bottleSpawnTimer = 0.0f;
};