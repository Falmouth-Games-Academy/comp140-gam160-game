#include "Bottle.h"
#include "Game.h"

void Bottle::OnSpawn() {
	// Load sprite
	sprite.LoadFrame("graphics/objects/bottles/Budweiser.png", Vec2(230.0f, 1522.0f), Vec2(0.3f, 0.3f));

	// Setup deafults
	collisionBox = Rect2(Vec2(0.0f, 0.0f), Vec2(553.0f, 2184.0f));

//	rotationSpeed = Math::randfloat(7.0f, 11.0f) * 360.0f * (float)Math::randsign(); // wheee

	isHurtable = true;
	health = 1.0f;
}

void Bottle::Update(float deltaTime) {
	Object::Update(deltaTime);
}

void Bottle::OnDeath() {
	Smash();
}

void Bottle::Smash() {
	if (IsBeingDestroyed()) {
		// Already shattered (probably!)
		return;
	}

	// Spawn glass fragments
	for (int i = 0; i < 9; ++i) {
		BottleFragment* fragment = game.SpawnObject<BottleFragment>();

		fragment->SetupFragment(this, i);
	}

	// Destroy self
	Destroy();
}

void BottleFragment::OnSpawn() {
	rotationSpeed = Math::randfloat(2.0f, 4.0f) * 360.0f * (float)Math::randsign();
	rotation = 0.0f;

	destroyTimer = 2.0f;
}

void BottleFragment::SetupFragment(const Bottle* parent, int index) {
	const Vec2 origins[] = {{135, 115}, {97, 195}, {105, 133}, {173, 223}, {177, 395}, {254, 369}, {273, 325}, {148, 319}, {321, 303}};
	const Vec2 relativeOrigins[] = {{53, -1371}, {-10, -1098}, {124, -1156}, {61, -806}, {-88, -428}, {194, -280}, {22, 81}, {-118, 442}, {174, 530}};
	const Vec2 scale = parent->GetSprite().GetDimensions() / parent->GetSprite().GetBaseDimensions();

	// Load the piece sprite
	sprite.ClearFrames();

	sprite.LoadFrame(StaticString<MAX_PATH>::FromFormat("graphics/objects/bottles/budweiser_shattered_%i.png", index), origins[index], scale);

	// Reposition so that it matches the original position of a bottle?
	position = parent->GetPosition() + relativeOrigins[index].Rotated(parent->GetRotation()) * scale;

	// Set its speed to be the same as the bottle, plus some speed from the centre, plus some upward motion so you can see it smash for half a second
	Vec3 extraVelocity = (origins[index] - (parent->GetSprite().GetBaseDimensions() * 0.5f)).Normalise() * Math::randfloat(6000.0f, 7000.0f) + Vec2(0.0f, -game.GetGravity() * 0.15f);

	velocity = parent->GetVelocity() + extraVelocity;
	//rotationSpeed = 0.0f;
}