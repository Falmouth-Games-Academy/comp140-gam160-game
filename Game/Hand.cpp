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
	sprite.LoadFrame("Graphics/hand1.png", Vec2(222.0f, 154.0f), Vec2(1.5f, 1.5f));

	// Setup collision box
	collisionBox = Rect2(7, 4, 259, 147);
	isSolid = true;

	// Set position
	position = Vec3(0.0f, -500.0f, 1.0f);

	direction = -1;
}

void Hand::Render() {
	// Draw the player
	game.GetCamera().RenderSprite(sprite, position + headBob, rotation, (direction == 1) ? true : false);

	// Draw relevant debug info
	if (DebugStringBox* debug = game.GetDebug()) {
		debug->DrawString(StaticString<80>::FromFormat("Player speed: %.2f, %.2f, %.2f", position.x, position.y, position.z));
		debug->DrawString(StaticString<80>::FromFormat("Player rotation: %.2f", rotation));
	}
}

void Hand::Update(float deltaTime) {
	const float minY = 0.0f, maxY = 900.0f, minX = 0.0f, maxX = 1800.0f;
	const float gravity = 1800.0f;

	// Update rotation
	Vec3 currentAccel = game.GetGesture().GetAverageAccel(25, 0);
	Vec3 lastAccel = game.GetGesture().GetAverageAccel(50, 25);

	float lastRotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-lastAccel.x, -lastAccel.z)) * Math::degs;
	rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-currentAccel.x, -currentAccel.z)) * Math::degs;

	// A tilt back must be a powerslide!!!
	// Note for calculating the angle: this is influenced heavily when the player is bouncing up and down
	// Todo, the average from a time when the range of force (max - min) is low?
	if (powerslideTimeRemaining > 0.0f) {
		powerslideTimeRemaining -= deltaTime;

		position.x += 1000.0f * direction * deltaTime;
	} else {
		// Detect powerslide

		if (Math::circlediff(lastRotation, rotation, 360.0f) >= 8.0f) {
			;//powerslideTimeRemaining = 0.3f;
		}
	}

	// Do gravity
	velocity.y += gravity * deltaTime;

	// Do gesture movement
	bool doFriction = false;
	GestureManager& gesture = game.GetGesture();
	auto accel = gesture.GetAccelHistory();
	const float xScale = 0.2f, yScale = 1.0f;
	static uint32 lastThresholdReached = 0;

	// Bob the head
	headBob.y = (gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 240.0f;

	// Check if the bounce force threshold was reached
	if (abs(gesture.GetAverageAccel(25, 0).z - gesture.GetAverageAccel(50, 0).z) >= 500.0f) {
		lastThresholdReached = game.GetFrameTimeMs();
	}

	// Keep moving as long as the force was above the threshold in the last 1000 ms
	if (game.GetFrameTimeMs() < lastThresholdReached + 1000) {
		position.x += abs((gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 10.0f) * direction * deltaTime;
	} else {
		// There hasn't been much movement for a while, so start slowing down
		doFriction = true;
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

	if (game.GetInput().IsKeyDown(SDLK_LEFT)) {
		velocity.x = -300.0f;
	} else if (game.GetInput().IsKeyDown(SDLK_RIGHT)) {
		velocity.x = 300.0f;
	}

	if (game.GetInput().IsKeyBooped(SDLK_SPACE)) {
		velocity.y = -1000.0f;
	}

	// Perform friction
	const float frictionRate = 1000.0f; // in pixel/s/s
	float velocitySign = Math::getsign(velocity.x);

	// Slow down if speed is > or < 0
	if (velocitySign > 0.0f) {
		velocity.x -= frictionRate * deltaTime;
	} else if (velocitySign < 0.0f) {
		velocity.x += frictionRate * deltaTime;
	}

	// Stop speed at 0 if we crossed between negative and positive speed
	if (Math::getsign(velocity.x) != velocitySign) {
		velocity.x = 0.0f;
	}

	// Perform final collision-checked movement
	this->Move(velocity * deltaTime);
}