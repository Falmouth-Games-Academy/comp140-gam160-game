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

	// Setup collision box
	collisionBox = Rect2(7, 4, 259, 147);
	isSolid = true;

	direction = -1;
}

void Hand::Render() {
	// Draw the player
	if (sprite.IsLoaded()) {
		Vec3 accel = game.GetGesture().GetAverageAccel(150, 0);
		float rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-accel.x, -accel.z)) * Math::degs;

		game.GetCamera().RenderSprite(sprite, position + headBob, rotation, (direction == 1) ? true : false);
	}

	position.z = 1.0f;
	game.GetCamera().RenderRectangle(position, Vec2(1.0f, 1.0f), Colour(0xFF00FF00));

	if (DebugStringBox* debug = game.GetDebug()) {
		debug->DrawString(StaticString<80>::FromFormat("Player speed: %.2f, %.2f, %.2f", position.x, position.y, position.z));
	}
}

void Hand::Update(float deltaTime) {
	const float minY = 0.0f, maxY = 900.0f, minX = 0.0f, maxX = 1800.0f;
	const float gravity = 1800.0f;

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

	// Doop doop friction
	bool doFriction = false;

	// Do gravity
	velocity.y += gravity * deltaTime;

	// Do gesture movement
	GestureManager& gesture = game.GetGesture();
	auto accel = gesture.GetAccelHistory();
	const float xScale = 0.2f, yScale = 1.0f;
	static uint32 lastThresholdReached = 0;

	headBob.y = (gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 240.0f;

	if (game.GetFrameTime() < lastThresholdReached + 1000) {
		position.x += abs((gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 10.0f) * direction * deltaTime;
	} else {
		// There hasn't been much movement for a while, so start slowing down
		doFriction = true;
	}

	if (abs(gesture.GetAverageAccel(25, 0).z - gesture.GetAverageAccel(50, 0).z) >= 500.0f) {
		lastThresholdReached = game.GetFrameTime();
	}

	printf("avg: %.2f cur: %.2f diff: %.2f\n", gesture.GetAverageAccel(1000, 0).z, gesture.GetAverageAccel(50, 0).z, headBob.y);

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