#include "Object.h"
#include "Player.h"
#include "Game.h"
#include "SDL.h"
#include "Image.h"

#include "Bottle.h" // Test bottle spawning
 
// Hand width: 9 1/2cm
Hand::~Hand() {
	return; // dur
}

void Hand::OnSpawn() {
	// Load player sprite
	sprite.LoadFrames("Graphics/player/laser/handzer", 5, Vec2(2496.0, 2102.0), Vec2(0.2f, 0.2f)/*Vec2(222.0f, 154.0f), Vec2(1.5f, 1.5f)*/);
	
	// Setup collision box
	collisionBox = Rect2(157, 701, 2509, 1661);
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
		debug->DrawString(StaticString<80>::FromFormat("Player speed: %.2f, %.2f, %.2f", velocity.x, velocity.y, velocity.z));
		debug->DrawString(StaticString<80>::FromFormat("Player rotation: %.2f", rotation));
		debug->DrawString(StaticString<80>::FromFormat("Mouth open angle: %.2f degrees", game.GetGesture().GetFlexAngle()));
	}
}

void Hand::Update(float deltaTime) {
	const float maxY = 900.0f;

	// Update rotation
	Vec3 currentAccel = game.GetGesture().GetAverageAccel(100, 0);
	float lastRotation = rotation;

	rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-currentAccel.y, -currentAccel.z)) * Math::degs - 35.0f;

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

	// Update laser power
	laserPower = Math::clamp(game.GetGesture().GetFlexAngle() / 90.0f, 0.0f, 1.0f);

	// Update mouse openness
	sprite.SetCurrentFrame(Math::clampmax(Math::round(laserPower * sprite.GetNumFrames(), 1.0f), (float)(sprite.GetNumFrames() - 1)));

	// Do gesture movement
	bool doFriction = false;
	bool doJump = false;
	GestureManager& gesture = game.GetGesture();

	// Calculate target speed based on rate and amplitude of hand bounces
	const float maxHandzerSpeed = 5000.0f;
	const float minBounceSpeed = 20000.0f;
	const float maxBounceSpeed = 240000.0f;
	const float minBounceAmplitude = 1000.0f;
	const float minAcceleration = 2000.0f;
	const float maxAcceleration = 5000.0f;
	const float minBounceAcceleration = minBounceSpeed;
	const float maxBounceAcceleration = 200000.0f;

	GestureManager::BounceInfo bounceInfo = game.GetGesture().CalculateBounceInfo(300, 0);

	float targetSpeed = 0.0f;
	float acceleration = 0.0f;
	
	if (bounceInfo.averageBounceAmplitude >= minBounceAmplitude) {
		float bounceSpeed = bounceInfo.averageBounceHz * bounceInfo.averageBounceAmplitude;

		targetSpeed = -Math::lerpfloat(Math::clamp(bounceSpeed, minBounceSpeed, maxBounceSpeed), 
							MinMax<float>(minBounceSpeed, maxBounceSpeed), MinMax<float>(0.0f, maxHandzerSpeed));

		acceleration = Math::lerpfloat(bounceSpeed, MinMax<float>(minBounceAcceleration, maxBounceAcceleration), MinMax<float>(minAcceleration, maxAcceleration));
	}

	if (bounceInfo.numBounces > 1) {
		rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-bounceInfo.lastCentralForce.y, -bounceInfo.lastCentralForce.z)) * Math::degs - 35.0f;
	}

	// Move in reverse if tilted back
	if (rotation > 8.0f) {
		targetSpeed = -targetSpeed;
	}

	// Jump if an unusually large amount of force was applied in a short time
	if (bounceInfo.maxBounceAmplitude >= 14000.0f) {
		doJump = true;
	}

	if (game.GetDebug()) {
		game.GetDebug()->DrawString(StaticString<80>::FromFormat("Bounce Num: %i   Hz: %.2f   Amp: %.2f", 
			bounceInfo.numBounces, bounceInfo.averageBounceHz, bounceInfo.averageBounceAmplitude));
		game.GetDebug()->DrawString(StaticString<80>::FromFormat("Bounce speed: %.2f", bounceInfo.averageBounceHz * bounceInfo.averageBounceAmplitude));
		game.GetDebug()->DrawString(StaticString<80>::FromFormat("Player target speed: %.2f  accel: %.2f", targetSpeed, acceleration));
		game.GetDebug()->DrawString(StaticString<80>::FromFormat("Amplitude deviation: %.2f", bounceInfo.maxBounceAmplitude - bounceInfo.averageBounceAmplitude));
	}

	if (abs(velocity.x) < abs(targetSpeed)) {
		velocity.x += deltaTime * acceleration * Math::getsign(targetSpeed);
	} else {
		doFriction = true;
	}

	// Bob the head
	headBob.y = (gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 240.0f;

	// Do gravity
	velocity.y += game.GetGravity() * deltaTime;

	// Do collision
	if (position.y > maxY) {
		position.y = maxY;
		velocity.y = 0.0f;
	}

	// Do debug keyboard movement
	if (game.GetInput().IsKeyDown(SDLK_LEFT)) {
		velocity.x = -900.0f;
	} else if (game.GetInput().IsKeyDown(SDLK_RIGHT)) {
		velocity.x = 900.0f;
	}

	if (game.GetInput().IsKeyBooped(SDLK_SPACE) || doJump) {
		velocity.y = -1000.0f;
	}

	// Perform friction
	if (doFriction) {
		const float frictionRate = 4000.0f; // in pixel/s/s
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
	}

	// Perform final collision-checked movement
	this->Move(velocity * deltaTime);
}

Vec3 Hand::SpritePointToWorldPoint(const Vec2& spritePoint) const {
	if (sprite.GetCurrentFrame()) {
		return sprite.GetCurrentFrame()->PixelToWorld(spritePoint, position + headBob, rotation);
	} else {
		return position + spritePoint;
	}
}