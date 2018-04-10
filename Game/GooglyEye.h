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
	// Sets the object that owns these googly eyes
	inline void SetParent(const Object* parent);

	// Sets the offset relative to the object's sprite frame, that these eyes are positioned
	inline void SetParentOffset(const Vec3& offset);

	// Sets dimensions of the eyeballs
	inline void SetPupilRadius(float32 radius);
	inline void SetEyeballRadius(float32 radius);

	// Sets the look force (a force where the eyes try to look, in m/s/s. For hyper-realistic googly eyes, set this to Vec2(0.0f, game.GetGravity());
	inline void SetLookForce(const Vec2& lookForce);

private:
	// Owner of the eyes
	const Object* parent = nullptr;

	// Positional eye offset, relative to the parent object in sprite pixels (if applicable)
	Vec3 parentOffset = Vec3(0.0f, 0.0f);

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
	Vec2 pupilLookForce;

	// Friction of the pupil, in pixels/sec/sec, when scraping against the edge of the eyeball
	float32 pupilFriction;
};

inline void GooglyEye::SetParentOffset(const Vec3& offset_) {
	this->parentOffset = offset_;
}

inline void GooglyEye::SetParent(const Object* parent_) {
	this->parent = parent_;
}

inline void GooglyEye::SetPupilRadius(float32 radius) {
	pupilRadius = radius;
}

inline void GooglyEye::SetEyeballRadius(float32 radius) {
	eyeRadius = radius;
}

inline void GooglyEye::SetLookForce(const Vec2& lookForce_) {
	pupilLookForce = lookForce_;
}
