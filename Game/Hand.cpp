#include "Object.h"
#include "Hand.h"
#include "Game.h"
#include "SDL.h"
#include "Image.h"
 
// Hand width: 9 1/2cm
Hand::~Hand() {
	return; // dur
}

void Hand::Spawn() {
	// Load player sprite
	sprite.Load("Graphics/hand1.png", Vec2(222.0f, 154.0f), Vec2(1.5f, 1.5f));

	direction = -1;
}

void Hand::Render() {
	// Draw the player
	if (sprite.IsLoaded()) {
		Vec3 accel = game.GetGesture().GetAverageAccel(150, 0);
		float rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-accel.x, -accel.z)) * Math::degs;

		game.GetCamera().RenderSprite(sprite, Vec3(position.x, position.y + headBob.y, 1.0f), rotation, (direction == 1) ? true : false);
	}
}

void Hand::Update(float deltaTime) {
	const float minY = 0.0f, maxY = 900.0f, minX = 0.0f, maxX = 1800.0f;

	// A tilt back must be a powerslide!!!
	Vec3 currentAccel = game.GetGesture().GetAverageAccel(25, 0);
	Vec3 lastAccel = game.GetGesture().GetAverageAccel(50, 25);
	float angle = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(currentAccel.x, currentAccel.z));
	float lastAngle = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(lastAccel.x, lastAccel.z));

	// When calculating the angle it's probably important to consider the divergence of the length of the vector at the time
	// If the entire vector is lengthening or shortening that implies additional up/down movement considering gravity

	if (powerslideTimeRemaining > 0.0f) {
		powerslideTimeRemaining -= deltaTime;

		position.x += 1000.0f * direction * deltaTime;
	} else {
		// Detect powerslide

		if (Math::circlediff(lastAngle, angle, Math::pi * 2.0f) >= 8.0f * Math::rads) {
			;//powerslideTimeRemaining = 0.3f;
		}
	}

	// Do gravity
	velocity.y += 1000.0f * deltaTime;
	velocity.y = 0.0f;

	// Do gesture movement
	GestureManager& gesture = game.GetGesture();
	auto accel = gesture.GetAccelHistory();
	const float xScale = 0.2f, yScale = 1.0f;
	static uint32 lastThresholdReached = 0;

	position.y = 700.0f;
	headBob.y = (gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 240.0f;

	if (game.GetFrameTime() < lastThresholdReached + 1000) {
		position.x += abs((gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 10.0f) * direction * deltaTime;
	}

	if (abs(gesture.GetAverageAccel(25, 0).z - gesture.GetAverageAccel(50, 0).z) >= 500.0f) {
		lastThresholdReached = game.GetFrameTime();
	}

	// Do collision
	if (position.y > maxY) {
		position.y = maxY;
		velocity.y = 0.0f;
	}

	if (position.y < minY) {
		position.y = minY;
		velocity.y = 0.0f;
	}

	if (position.x < minX) {
		position.x = minX;
		velocity.x = 0.0f;
		direction = 1;
	}
	if (position.x > maxX) {
		position.x = maxX;
		velocity.x = 0.0f;
		direction = -1;
	}

	// Perform final movement
	position += velocity * deltaTime;
}

const Vec2& Hand::GetPosition() const {
	return position;
}
