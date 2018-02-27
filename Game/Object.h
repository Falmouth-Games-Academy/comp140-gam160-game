#pragma once
#include "Sprite.h"
#include "Render.h"

class Object {
public:
	Object() : position(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), sprite(), isSolid(false) {};

public:
	enum Type {
		BackgroundLayer = 0,
		Hand = 1,
		NumTypes
	};

	virtual Type GetType() const = 0;

public:
	// Returns current position
	inline const Vec3& GetPosition() const;

	// Attempts to move to a position. If teleport is false, collision detection is applied if the object is solid
	bool Move(const Vec3& moveOffset, bool teleport = false);

	inline void SetPosition(const Vec3& position);

	// Returns the sprite
	inline const Sprite& GetSprite() const;

	// Sets the size of the object in pixels
	inline void SetSize(const Vec2& size);

	// Returns the size of the object in pixels
	inline const Vec2& GetSize() const;

	// Sets the collision box, or if the collision box is nullptr, assume non-solid
	void SetCollision(const Rect2* newBox);

	// Renders the object's collision box as a blue bounding border
	void RenderCollisionBox() const;

	bool IsColliding(const Object& otherObject);

protected:
	// Current sprite
	Sprite sprite;

	// Position and speed of the object
	Vec3 position;
	Vec3 velocity;

	// Dimensions of the collision box, relative to this object's unscaled sprite pixels
	Rect2 collisionBox;
	bool8 isSolid;
};

inline const Vec3& Object::GetPosition() const {
	return position;
}

inline bool Object::Move(const Vec3& moveOffset, bool teleport) {
	position += moveOffset;
	return true;
}

inline void Object::SetPosition(const Vec3& position) {
	this->position = position;
}

inline const Sprite& Object::GetSprite() const {
	return sprite;
}

inline void Object::SetSize(const Vec2& size) {
	sprite.SetScale(size / sprite.GetBaseDimensions());
}

inline const Vec2& Object::GetSize() const {
	return sprite.GetDimensions();
}

inline void Object::SetCollision(const Rect2* newBox) {
	if (newBox) {
		collisionBox = *newBox;
	}
	else {
		isSolid = false;
	}
}