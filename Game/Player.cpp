#include "Object.h"
#include "Player.h"
#include "Game.h"
#include "SDL.h"
#include "Image.h"
#include "GooglyEye.h"
#include "Laser.h"
#include "GameStateDed.h"
 
// Hand width: 9 1/2cm
Hand::~Hand() {
	return; // dur
}

void Hand::OnSpawn() {
	// Load player sprite
	//sprite.LoadFrames("Graphics/player/laser/handzer", 5, Vec2(2496.0, 2102.0), Vec2(0.2f, 0.2f));
	sprite.LoadFrames("Graphics/player/laser/handzer", 9, Vec2(1268.0f, 988.0f), Vec2(0.5f, 0.5f));
	
	// And arm sprite
	armSprite.LoadFrame("graphics/player/laser/arm_1.png", Vec2(190.0f, 180.0f), Vec2(0.5f, 0.5f));

	// Setup collision box
	collisionBox = Rect2(187.0f, 357.0f, 1217.0f, 760.0f);
	collisionFlags = SolidOverlaps;

	// Setup other defaults
	isHurtable = true;
	defaultHurtInvincibilityTime = 1.0f;

	updateFlags = UpdateHurtFlashes | UpdateInvincibilityTimer | UpdatePhysics;

	// Start going left!
	direction = -1;

	// Create googly eyes
	leftEye = game.SpawnObject<GooglyEye>();
	rightEye = game.SpawnObject<GooglyEye>();

	leftEye->SetParent(this);
	rightEye->SetParent(this);

	leftEye->SetParentOffset(Vec3(1104.0f - 100.0f, 348.0f, 0.2f));
	rightEye->SetParentOffset(Vec3(1104.0f, 348.0f, 0.0f));

	// Create the laser
	laser = game.SpawnObject<Laser>();

	// Do first respawn (which is technically a spawn but imma let it finish)
	Respawn();
}

void Hand::OnDestroy() {
	leftEye->Destroy();
	rightEye->Destroy();

	laser->Destroy();
}

void Hand::OnDeath() {
	// Set the game state to the death state
	game.SetGameState<GameStateDed>();
}

void Hand::Render() {
	// Draw the arm
	game.GetCamera().RenderSprite(armSprite, position + headBob, 0.0f, (direction == 1) ? true : false);

	// Draw the player
	game.GetCamera().RenderSprite(sprite, position + headBob, rotation, (direction == 1) ? true : false);

	// Draw relevant debug info
	if (DebugStringBox* debug = game.GetDebugBox()) {
		debug->DrawString(StaticString<80>::FromFormat("Player speed: %.2f, %.2f, %.2f", velocity.x, velocity.y, velocity.z));
		debug->DrawString(StaticString<80>::FromFormat("Player rotation: %.2f", rotation));
		debug->DrawString(StaticString<80>::FromFormat("Mouth open angle: %.2f degrees", game.GetGesture().GetFlexAngle()));
	}
}

void Hand::Update(float deltaTime) {
	// Update rotation
	Vec3 currentAccel = game.GetGesture().GetAverageAccel(200, 0);
	float lastRotation = rotation;

	rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-currentAccel.y, -currentAccel.z)) * Math::degs;

	// Todo maybe: powerslides
	// Update laser power
	laserPower = game.GetGesture().GetHandOpenness();

	// Hack: Fix infinite power glitch
	if (laserPower != laserPower) {
		laserPower = 0.0f;
	}

	// Change sprite to open or closed
	sprite.SetCurrentFrame(Math::round(laserPower * (sprite.GetNumFrames() - 1), 1.0f));

	// Do gesture movement
	bool doFriction = false;
	bool doJump = false;
	GestureManager& gesture = game.GetGesture();

	// Calculate target speed based on rate and amplitude of hand bounces
	const float maxHandzerSpeed = 5000.0f;
	const float minBounceSpeed = 20000.0f, maxBounceSpeed = 240000.0f;
	const float minAcceleration = 2000.0f, maxAcceleration = 5000.0f;
	const float minBounceAcceleration = minBounceSpeed, maxBounceAcceleration = 200000.0f;
	const float minBounceAmplitude = 2000.0f;
	const float bounceTiltAmpThreshold = 4000.0f; // confusing, but this is the minimum bounce amp before the player's angle is calculated using the bounces
	const int bounceTiltNumThreshold = 3;
	const float jumpAmplitude = 18000.0f;

	// Bounce along!
	GestureManager::BounceInfo bounceInfo = game.GetGesture().CalculateBounceInfo(300, 0);

	float targetSpeed = 0.0f;
	float acceleration = 0.0f;
	
	if (bounceInfo.averageBounceAmplitude >= minBounceAmplitude) {
		float bounceSpeed = bounceInfo.averageBounceHz * bounceInfo.averageBounceAmplitude;

		targetSpeed = -Math::lerpfloat(Math::clamp(bounceSpeed, minBounceSpeed, maxBounceSpeed), 
							MinMax<float>(minBounceSpeed, maxBounceSpeed), MinMax<float>(0.0f, maxHandzerSpeed));

		acceleration = Math::lerpfloat(bounceSpeed, MinMax<float>(minBounceAcceleration, maxBounceAcceleration), MinMax<float>(minAcceleration, maxAcceleration));

		if (bounceInfo.numBounces > bounceTiltNumThreshold && bounceInfo.averageBounceAmplitude >= bounceTiltAmpThreshold) {
			rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-bounceInfo.lastCentralForce.y, -bounceInfo.lastCentralForce.z)) * Math::degs;
		}
	}

	// Move in reverse if tilted back
	if (rotation > 8.0f) {
		targetSpeed = -targetSpeed;
	}

	// Jump if an unusually large amount of force was applied in a short time
	if (bounceInfo.maxBounceAmplitude >= jumpAmplitude) {
		doJump = true;
	}

	// Draw debug stuff?
	if (game.GetDebugBox()) {
		game.GetDebugBox()->DrawString(StaticString<80>::FromFormat("Bounce Num: %i   Hz: %.2f   Amp: %.2f", 
			bounceInfo.numBounces, bounceInfo.averageBounceHz, bounceInfo.averageBounceAmplitude));
		game.GetDebugBox()->DrawString(StaticString<80>::FromFormat("Bounce speed: %.2f", bounceInfo.averageBounceHz * bounceInfo.averageBounceAmplitude));
		game.GetDebugBox()->DrawString(StaticString<80>::FromFormat("Player target speed: %.2f  accel: %.2f", targetSpeed, acceleration));
		game.GetDebugBox()->DrawString(StaticString<80>::FromFormat("Amplitude deviation: %.2f", bounceInfo.maxBounceAmplitude - bounceInfo.averageBounceAmplitude));
	}

	if (abs(velocity.x) < abs(targetSpeed)) {
		velocity.x += deltaTime * acceleration * Math::getsign(targetSpeed);
	} else {
		doFriction = true;
	}

	// Bob the head
	headBob = Vec2(0.0f, -1.0f) * (gesture.GetAccelAtTime(0).yz.Length() - 9800.0f) * 0.03f;

	// Jump
	if (isOnGround && (game.GetInput().IsKeyBooped(SDLK_SPACE) || doJump)) {
		velocity.y = -game.GetGravity() * 0.25f;
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

	// Perform super update
	Object::Update(deltaTime);

	// Update eye positions
	leftEye->SetParentOffset(Vec3(1104.0f - 100.0f, 348.0f, 0.2f).Lerped(Vec3(900.0f - 100.0f, 476.0f, 0.2f), laserPower));
	rightEye->SetParentOffset(Vec3(1104.0f, 348.0f, 0.0f).Lerped(Vec3(900.0f, 476.0f, 0.0f), laserPower));

	// Update eye direction
	const float googlyStrength = 4000.0f;
	leftEye->SetLookForce(Vec2(-1.0f, 0.0f).Rotated(rotation - 40.0f).Lerped(Vec2(1.0f, -1.0f), laserPower) * googlyStrength);
	rightEye->SetLookForce(Vec2(-1.0f, 0.0f).Rotated(rotation).Lerped(Vec2(1.0f, -1.0f), laserPower) * googlyStrength);

	// Not to mention pupil size
	leftEye->SetPupilRadius(Math::lerpfloat(laserPower, MinMax<float>(50.0f, 85.0f)));
	rightEye->SetPupilRadius(Math::lerpfloat(laserPower, MinMax<float>(42.0f, 70.0f)));

	leftEye->SetEyeballRadius(Math::lerpfloat(laserPower, MinMax<float>(90.0f, 130.0f)));
	rightEye->SetEyeballRadius(Math::lerpfloat(laserPower, MinMax<float>(80.0f, 100.0f)));
}

void Hand::Respawn() {
	// Just reappear at the start of the level
	position = game.GetLevel().GetPlayerStart();

	// Restore health
	health = 100.0f;

	// I think that's all?
}

Vec3 Hand::SpritePointToWorldPoint(const Vec2& spritePoint) const {
	if (sprite.GetCurrentFrame()) {
		return sprite.GetCurrentFrame()->PixelToWorld(spritePoint, position + headBob, rotation);
	} else {
		return position + spritePoint;
	}
}