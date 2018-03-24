#include "Bottle.h"
#include "Game.h"

void Bottle::OnSpawn() {
	sprite.LoadFrame("graphics/objects/bottles/Budweiser.png", Vec2(230.0f, 1522.0f), Vec2(0.3f, 0.3f));

	position = game.GetPlayer().GetPosition() + Vec3(Math::randfloat(-1000.0f, 1000.0f), -500.0f, 0.0f);

	rotationSpeed = Math::randfloat(7.0f, 11.0f) * 360.0f * (float)Math::randsign();

	destroyTimer = 4.0f;
}

void Bottle::Update(float deltaTime) {
	Object::Update(deltaTime);

	if (!IsBeingDestroyed()) {
		// Move
		if (!Move(velocity * deltaTime, false)) {
			Smash();
		}
	}
}

void Bottle::Smash() {
	if (IsBeingDestroyed()) {
		// Already shattered (probably!)
		return;
	}

	// Spawn glass fragments
	for (int i = 0; i < 8; ++i) {
		BottleFragment* fragment = game.SpawnObject<BottleFragment>();

		fragment->SetupFragment(this, i);
	}

	// Destroy self
	Destroy();
}

void BottleFragment::OnSpawn() {
	rotationSpeed = Math::randfloat(7.0f, 11.0f) * 360.0f * (float)Math::randsign();
	rotation = 0.0f;

	destroyTimer = 2.0f;
}

void BottleFragment::SetupFragment(const Bottle* parent, int index) {
	const Vec2 origins[] = {{321.0f, 109.0f}, {258, 410}, {381, 299}, {332, 682}, {155, 1046}, {443, 1191}, {274, 1571}, {140, 1890}, {464, 1996}};
	const Vec2 scale = parent->GetSprite().GetDimensions() / parent->GetSprite().GetBaseDimensions();

	// Load the piece sprite
	sprite.ClearFrames();

	sprite.LoadFrame(StaticString<MAX_PATH>::FromFormat("graphics/objects/bottles/budweiser_shattered_%i.png", index), origins[index], scale);

	// Reposition so that it matches the original position of a bottle?
	position = parent->GetPosition() + origins[index] * scale;

	// Set its speed to be the same as the bottle, plus some speed from the centre
	Vec3 extraVelocity = (origins[index] - (parent->GetSprite().GetBaseDimensions() * 0.5f)).Normalise();

	velocity = parent->GetVelocity() + extraVelocity * Math::randfloat(100.0f, 150.0f);
}