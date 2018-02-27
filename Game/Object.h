#pragma once
#include "Sprite.h"
#include "Render.h"

class Object {
public:
	Object() : position(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), sprite() {};

	enum Type {
		Hand = 0,
		NumTypes
	};

	virtual Type GetType() const = 0;

	// Returns current position
	inline const Vec3& GetPosition() const;

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
};

inline const Vec3& Object::GetPosition() const {
	return position;
}