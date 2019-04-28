#pragma once

#include "SObject.h"

class SProjectile : public SObject
{
public:
	SProjectile();
	~SProjectile();

	// Perform translation
	void Update();

	// Perform collision

private:
	// Damage to deal
	int damage;

	// Movement direction
	glm::vec3 direction;
};

