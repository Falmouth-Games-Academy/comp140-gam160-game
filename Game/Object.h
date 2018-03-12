#pragma once
#include "Sprite.h"
#include "Render.h"

class Object {
public:
	Object() : position(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), sprite(), isSolid(false) {};

public:
	enum Type {
		BackgroundLayerType = 0,
		HandType = 1,
		LaserType = 2,
		NumTypes
	};

	virtual void Spawn() {return;}

	virtual void Update(float deltaTime) {return;}
	virtual void Render() {return;}

	virtual Type GetType() const = 0;

public:
	// Returns current position
	inline const Vec3& GetPosition() const;

	// Sets the position of the object (teleport)
	inline void SetPosition(const Vec3& position);

	// Attempts to move to a position. If teleport is false, collision detection is applied if the object is solid
	// Returns whether the move was fully successful without any collisions
	bool Move(const Vec3& moveOffset, bool doAffectVelocity = true, bool teleport = false);

	// Returns the object's rotation in degrees
	inline float32 GetRotation() const;

	// Returns the sprite
	inline const Sprite& GetSprite() const;

	// Sets the size of the object in pixels (calculates scale)
	inline void SetSize(const Vec2& size);

	// Returns the scaled size of the object in pixels
	inline const Vec2 GetSize() const;

	// Sets the scale of the object
	inline void SetScale(const Vec2& scale);

	// Sets the collision box, or if the collision box is nullptr, set as non-solid
	void SetCollision(const Rect2* newBox, bool newIsSolid = true);

	// Renders the object's collision box as a blue bounding border
	void RenderCollisionBox() const;

	// Returns whether this object is colliding with another object.
	/* If true, and borderOffsets is non-null, borderOffsets is set to the possible offsets you could move the object by to nullify the collision, 
	   where e.g. the top is the offset you can add to push your top away from the bottom of the other object. */
	bool IsColliding(const Object& otherObject, Bounds2* borderOffsets = nullptr);

protected:
	// Current sprite
	Sprite sprite;

	// Position and speed of the object
	Vec3 position;
	Vec3 velocity;

	// Rotation of the object, in degrees
	float32 rotation;

	// Dimensions of the collision box, relative to this object's unscaled sprite pixels
	Rect2 collisionBox;
	bool8 isSolid;
};

inline const Vec3& Object::GetPosition() const {
	return position;
}

inline void Object::SetPosition(const Vec3& position) {
	this->position = position;
}

inline float32 Object::GetRotation() const {
	return rotation;
}

inline const Sprite& Object::GetSprite() const {
	return sprite;
}

inline void Object::SetScale(const Vec2& scale) {
	for (SpriteFrame* frame : sprite.GetFrames()) {
		frame->SetScale(scale);
	}
}

inline void Object::SetSize(const Vec2& size) {
	if (sprite.GetCurrentFrame()) {
		Vec2 scale = size / sprite.GetCurrentFrame()->GetBaseDimensions();
	
		for (SpriteFrame* frame : sprite.GetFrames()) {
			frame->SetScale(scale);
		}
	}
}

inline const Vec2 Object::GetSize() const {
	if (sprite.GetCurrentFrame()) {
		// Todo: Erm, what to do when different frames are different sizes?
		return sprite.GetCurrentFrame()->GetDimensions();
	} else {
		// No sprite image, no size!?
		return Vec2(0.0f, 0.0f);
	}
}

inline void Object::SetCollision(const Rect2* newBox, bool newIsSolid) {
	if (newBox) {
		collisionBox = *newBox;
		isSolid = newIsSolid;
	} else {
		isSolid = false;
	}
}