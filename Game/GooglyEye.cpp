#include "Game.h"
#include "GooglyEye.h"

void GooglyEye::OnSpawn() {
	// Load eye components
	sprite.LoadFrame("graphics/player/eye/eye.png", Vec2(128.0f, 128.0f));
	sprite.LoadFrame("graphics/player/eye/pupil.png", Vec2(30.0f, 30.0f));

	// Setup vars
	eyeRadius = 75.0f;
	pupilRadius = 55.0f;
	pupilEdgeScale = 0.7f;

	pupilLookForce = Vec2(0.0f, 1.0f) * game.GetGravity();
	pupilFriction = 800.0f;
}

void GooglyEye::Update(float deltaTime) {
	Vec3 lastPosition = position;

	// Update position to mouse position (for testing purposes)
	//position = game.GetCamera().ScreenToWorld(Vec3(game.GetInput().GetMousePosition(), 1.0f));

	// Update position to player position
	position = game.GetPlayer().SpritePointToWorldPoint(parentOffset.xy) + Vec3(0.0f, 0.0f, parentOffset.z);

	const float forceMultiplier = 0.4f;

	pupilPosition += (lastPosition.xy - position.xy) * forceMultiplier;

	// Pupil physics!!
	UpdatePupilPhysics(deltaTime);

	// Update scaling
	float pupilXScale = Math::lerpfloat(pupilPosition.Length(), MinMax<float>(eyeRadius - pupilRadius, 0.0f), MinMax<float>(pupilEdgeScale, 1.0f));

	sprite.GetFrame(0)->SetScale(Vec2(eyeRadius * 2.0f / sprite.GetFrame(0)->GetBaseDimensions().x));
	sprite.GetFrame(1)->SetScale(Vec2(pupilRadius * 2.0f / sprite.GetFrame(1)->GetBaseDimensions().x) * Vec2(pupilXScale, 1.0f));
}

void GooglyEye::Render() {
	// Render the eyeball
	game.GetCamera().RenderSpriteFrame(*sprite.GetFrame(0), position);
	
	// Render the pupil
	game.GetCamera().RenderSpriteFrame(*sprite.GetFrame(1), position + pupilPosition, 90.0f + Vec2::Direction(Vec2(0.0f, 0.0f), pupilPosition) * Math::degs);
}

void GooglyEye::UpdatePupilPhysics(float deltaTime) {
	// Do gravity
	pupilVelocity += pupilLookForce * deltaTime;

	// Move the pupil
	pupilPosition += pupilVelocity * deltaTime;

	// Respond to collisions with the edge of the doodad
	if (pupilPosition.Length() >= eyeRadius - pupilRadius) {
		Vec2 pushBackVector = pupilPosition * (pupilPosition.Length() - (eyeRadius - pupilRadius)) / pupilPosition.Length();

		pupilPosition -= pushBackVector;

		if (deltaTime > 0.0f) {
			pupilVelocity -= pushBackVector / deltaTime;
		}

		// Apply friction
		pupilVelocity *= (pupilVelocity.Length() - pupilFriction * deltaTime) / pupilVelocity.Length();
	}

	if (pupilVelocity.x != pupilVelocity.x) {
		pupilVelocity = Vec2(0.0f); // Hack: inf vector due to 0 deltaTime
	}
}