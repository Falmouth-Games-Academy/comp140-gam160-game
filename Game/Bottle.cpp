#include "Bottle.h"
#include "Game.h"

void Bottle::Spawn() {
	sprite.LoadFrame("graphics/objects/bottles/Budweiser.png", Vec2(230.0f, 1522.0f), Vec2(0.3f, 0.3f));

	position = game.GetPlayer().GetPosition() + Vec3(Math::randfloat(-1000.0f, 1000.0f), -500.0f, 0.0f);

	spinRate = Math::randfloat(7.0f, 11.0f) * 360.0f * (float)Math::randsign();
}

void Bottle::Update(float deltaTime) {
	// Update spin
	rotation = Math::circleclamp(rotation + spinRate * deltaTime, 360.0f);

	// Update gravity
	velocity.y = Math::clamp(velocity.y + 3000.0f * deltaTime, -2000.0f, 6000.0f);

	// Move
	if (!Move(velocity * deltaTime, false)) {
		Shatter();
	}

	// Todo: smash if not moved successfully
}

void Bottle::Shatter() {
	for (int i = 0; i < 9; ++i) {
		BottleFragment* fragment = game.SpawnObject<BottleFragment>();

		fragment->SetupFragment(this, i);
	}
}

void BottleFragment::Spawn() {
	spinRate = Math::randfloat(7.0f, 11.0f) * 360.0f * (float)Math::randsign();
}

void BottleFragment::Update(float deltaTime) {
	// Update gravity
	velocity.y = Math::clamp(velocity.y + 3000.0f * deltaTime, -2000.0f, 6000.0f);

	// Update movement
	Move(velocity * deltaTime, false);
}

void BottleFragment::SetupFragment(const Bottle* parent, int index) {
	const Vec2 origins[] = {{321.0f, 109.0f}, {258, 410}, {381, 299}, {332, 682}, {155, 1046}, {443, 1191}, {274, 1571}, {140, 1890}, {464, 1996}};

	// Load the piece sprite
	sprite.ClearFrames();

	sprite.LoadFrame(StaticString<MAX_PATH>::FromFormat("graphics/objects/bottles/budweiser_shatter_%i.png", index), origins[index]);

	// Reposition so that it matches the original position of a bottle?
	position -= origins[index];
}