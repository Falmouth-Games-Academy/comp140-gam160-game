#pragma once
#include "Sprite.h"
#include "Render.h"

// Base class for interactive objects in-game
class Object {
public:
	// Default constructor: spawn anywhere, with random variables, durh
	Object() = default;

	// Move constructor
	Object(Object&& moved) = default;

public:
	enum Type {
		BackgroundLayerType = 0,
		HandType,
		BottleType,
		BottleFragmentType,
		LaserType,
		GooglyEyeType,
		BigfootType,
		NumTypes
	};

	// Anonymous constructor. May return nullptr if the type isn't in the type database
	static inline Object* ConstructByType(Object::Type type);

	// Returns the object type. Must be overridden by the object class type
	virtual Type GetType() const = 0;

public:
	// Destroyer
	inline void Destroy();

public:
	// Events
	// Called when the object spawns. Default action: None
	virtual void OnSpawn();

	// Called when the object is destroyed. Default action: None
	virtual void OnDestroy();

public:
	// Main functions
	// Updates the object. Default action: Move the object by its velocity, spin it by its rotation speed, and countdown destroy timer
	virtual void Update(float deltaTime);

	// Renders the object. Default action: Render the sprite at the current position
	virtual void Render();

public:
	// Combat virtuals
	virtual void OnDamage();

public:
	// Positioning and movement
	// Returns current position
	inline const Vec3& GetPosition() const;

	// Sets the position of the object (teleport)
	inline void SetPosition(const Vec3& position);

	// Attempts to move to a position. If teleport is false, collision detection is applied if the object is solid
	// Returns whether the move was fully successful without any collisions
	bool Move(const Vec3& moveOffset, bool teleport = false);

	// Returns the object's velocity
	inline const Vec3& GetVelocity() const;

	// Sets the object's raw velocity. Useful for spawning projectiles, etc
	inline void SetVelocity(const Vec3& velocity);

public:
	// Misc getters and setters
	// Returns the object's rotation in degrees
	inline float32 GetRotation() const;

	// Sets the object's rotation in degrees
	inline void SetRotation(float32 rotation);

	// Returns the sprite
	inline const Sprite& GetSprite() const;

	// Sets the size of the object in pixels (calculates scale)
	inline void SetSize(const Vec2& size);

	// Returns the scaled size of the object in pixels
	inline const Vec2 GetSize() const;

	// Sets the scale of the object
	inline void SetScale(const Vec2& scale);

	// Returns whether the objects is gon die
	inline bool IsBeingDestroyed() const;

public:
	// Collision boxes and detection
	enum CollisionFlags {
		SolidEnv = 1,			// Solid against the environment
		SolidObjs = 2,			// Solid against objects
		Solid = 3,				// Blocked by other objects and blocks other objects (unless PhaseSelf is set); sends and receives collision events
		OverlapEnv = 4,			// Sends and receives collision events involving the environment
		OverlapObjs = 8,		// Sends and receives collision events involving other objects
		SolidOverlaps = 15,     // Solid and overlaps everything
		PreserveXVelocity = 16, // Conserves X velocity after colliding with something solid
		PreserveYVelocity = 32, // Conserves Y velocity after colliding with something solid
	};

	// Sets the collision box, if non-null, and sets whether the object is solid
	void SetCollisionBox(const Rect2& newBox);

	// Gets the collision box, or if non-solid, returns nullptr
	inline const Rect2& GetCollisionBox() const;

	// Sets the collision flags
	inline void SetCollisionFlags(uint32 flags);

	// Returns collision flags
	inline uint32 GetCollisionFlags() const;

	// Renders the object's collision box as a blue bounding border
	void RenderCollisionBox() const;

	// Returns whether this object is colliding with another object.
	/* If true, and borderOffsets is non-null, borderOffsets is set to the possible offsets you could move the object by to nullify the collision, 
	   where e.g. the top is the offset you can add to push your top away from the bottom of the other object. */
	bool IsColliding(const Object& otherObject, Bounds2* borderOffsets = nullptr);

public:
	// Health and combat
	// Changes the health by healthData. hurtInvincibilityTime gives an invincibility timer if not -1.0f. If it is -1.0f, the default invincibility time is used.
	void ChangeHealth(float32 healthDelta, float32 hurtInvincibilityTime = -1.0f);

public:
	// Sprite position calculations
	// Converts a pixel position on the object's sprite to a world position, considering the object's rotation and scale
	virtual Vec3 SpritePointToWorldPoint(const Vec2& spritePoint) const;

public:
	// Called when the object overlaps another object or scene layer
	virtual void OnOverlap(Object& otherObject);

public:
	// Sets the information applicable to the object when it first spawned
	inline void SetSpawnInfo(const Vec3& spawnPosition, bool isPersistent);
	
	inline const Vec3& GetSpawnPosition() const;

	// Returns whether the object is persistent (should be saved into the map object data)
	inline bool IsPersistent() const;

protected:
	// Current sprite
	Sprite sprite;

	// Position and speed of the object
	Vec3 position = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 velocity = Vec3(0.0f, 0.0f, 0.0f); // pixels/sec

	// Rotation of the object, in degrees
	float32 rotation = 0.0f;
	float32 rotationSpeed = 0.0f; // deg/sec

	// Flags determining what actions Update will undertake
	enum UpdateFlags {
		UpdateGravity = 1,
		UpdateRotation = 2,
		UpdatePhysics = UpdateGravity | UpdateRotation,
		UpdateDeathTimer = 4,
		UpdateHurtFlashes = 8,
		UpdateInvincibilityTimer = 16,

		UpdateAll = 0xFFFF,
	};

	uint32 updateFlags = UpdateAll;

	// Dimensions of the collision box, relative to this object's unscaled sprite pixels
	Rect2 collisionBox;

	// Collision flags that specify collision behaviours
	uint32 collisionFlags;

	// Whether the object is on the ground (simulated objects only)
	bool8 isOnGround = false;

	// Health of the object (if applicable)
	float32 health = 100.0f;

	// Whether or not this object uses health
	bool8 isHurtable = false;

	// A timer representing the red flash indicated when damage is dealt
	float32 hurtFlashTimer = 0.0f;
	float32 maxHurtFlashTimer = 0.2f;

	// A timer counting down the short invincibility period after being hurt
	float32 hurtInvincibilityTimer = 0.0f;

	// The default invincibility time after being hurt, in seconds
	float32 defaultHurtInvincibilityTime = 0.0f;

	// If above 0, this is a timer, which by Object::Update counts down to 0. Once 0 is reached, the object self-destructs
	float32 destroyTimer;

	// Whether the object should be saved into the map object data
	bool8 isPersistent = false;

	// The position at which the object should spawn
	Vec3 spawnPosition;

private:
	// Whether the object is being destroyed next frame
	bool8 isBeingDestroyed = false;

private:
	// Object type database stuff follows (used to spawn an object when only the type enum is known)
	struct SpawnerDatabase {
		// Initialises the database
		SpawnerDatabase();

		Object* (*spawners[Object::NumTypes])();
	};

	static SpawnerDatabase spawners;
};

inline Object* Object::ConstructByType(Object::Type type) {
	if (spawners.spawners[type]) {
		// Call the spawner function which will construct the object
		return spawners.spawners[type]();
	} else {
		return nullptr;
	}
}

inline void Object::Destroy() {
	if (!isBeingDestroyed) {
		// Call destruction event
		OnDestroy();

		// Prepare for destruction at the end of the frame
		isBeingDestroyed = true;
	}
}

inline const Vec3& Object::GetPosition() const {
	return position;
}

inline void Object::SetPosition(const Vec3& position_) {
	this->position = position_;
}

inline const Vec3& Object::GetVelocity() const {
	return velocity;
}

inline void Object::SetVelocity(const Vec3& velocity_) {
	this->velocity = velocity_;
}

inline float32 Object::GetRotation() const {
	return rotation;
}

inline void Object::SetRotation(float32 rotation) {
	this->rotation = rotation;
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

inline bool Object::IsBeingDestroyed() const {
	return isBeingDestroyed;
}

inline void Object::SetCollisionBox(const Rect2& newBox) {
	collisionBox = newBox;
}

inline const Rect2& Object::GetCollisionBox() const {
	return collisionBox;
}

inline uint32 Object::GetCollisionFlags() const {
	return collisionFlags;
}

inline void Object::SetCollisionFlags(uint32 flags) {
	collisionFlags = flags;
}

inline void Object::SetSpawnInfo(const Vec3& spawnPosition, bool isPersistent) {
	this->isPersistent = isPersistent;
	this->spawnPosition = spawnPosition;
}

inline bool Object::IsPersistent() const {
	return isPersistent;
}

inline const Vec3& Object::GetSpawnPosition() const {
	return spawnPosition;
}