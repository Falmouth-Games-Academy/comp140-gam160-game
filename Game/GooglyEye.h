#pragma once
#include "Object.h"

class GooglyEye : public Object {
public:
	// Events
	// Called when the object spawns. Default action: None
	void OnSpawn() override;

	// Called when the object is destroyed. Default action: None
	void OnDestroy() override {return;}

public:
	// Main functions
	// Updates the googly eye's delicious googly physics
	void Update(float deltaTime) override;

	// Renders the googly eye
	void Render() override;

	Type GetType() const override {return GooglyEyeType;};

public:
	// Pupil physics update
	void UpdatePupilPhysics(float deltaTime);

public:
	inline void SetPlayerOffset(const Vec3& offset);

private:
	// Positional eye offset, relative to the player's central singular cyclops eye socket, if it had one, in player sprite pixels.
	Vec3 playerOffset = Vec3(0.0f, 0.0f);

	// Radius of eye and pupil
	float32 eyeRadius;
	float32 pupilRadius;

	// Scale factor of the pupil when it's at the edge of the eye (squish effect)
	float32 pupilEdgeScale;

	// Current position of the pupil, relative to the centre of the eyeball
	Vec2 pupilPosition;

	// Current velocity of the pupil
	Vec2 pupilVelocity;

	// Direction that the pupil tends to rest at (is actually a replacement for gravity)
	Vec2 pupilRestingDirection;

	// Friction of the pupil, in pixels/sec/sec, when scraping against the edge of the eyeball
	float32 pupilFriction;
};

inline void GooglyEye::SetPlayerOffset(const Vec3& offset) {
	this->playerOffset = offset;
}