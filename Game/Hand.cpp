#include "Object.h"
#include "Hand.h"
#include "Game.h"
#include "SDL.h"
#include "Image.h"
 
Hand::~Hand() {
	if (sprite) {
		SDL_DestroyTexture(sprite);
	}
}

void Hand::Spawn() {
	// Load player sprite
	Image handImage("Graphics/hand1.png");

	if (handImage.IsLoaded()) {
		sprite = handImage.CreateSDLTexture(game.GetRenderer());
		spriteWidth = handImage.GetDimensions().width;
		spriteHeight = handImage.GetDimensions().height;
	}

	direction = -1;
}

void Hand::Render() {
	// Draw the player
	Vec2 spriteScale(1.5f, 1.5f);
	SDL_Point originOfRotation = {222.0f * spriteScale.x, 154.0f * spriteScale.y};
	Vec2 origin(position.x + originOfRotation.x, position.y + originOfRotation.y);

	if (sprite) {
		Vec3 accel = game.GetGesture().GetAverageAccel(150, 0);
		SDL_Rect spriteSplatRect = {position.x, position.y, spriteWidth * spriteScale.x, spriteHeight * spriteScale.y};
		float rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-accel.x, -accel.z)) * Math::degs;

		SDL_RenderCopyEx(game.GetRenderer(), sprite, nullptr, &spriteSplatRect, rotation, &originOfRotation, direction == -1 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
	}

	// Draw the force vector
	Vec3 accel = game.GetGesture().GetAverageAccel(200, 0);
	const float forceScale = 0.03f;

	SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawLine(game.GetRenderer(), (int)origin.x, (int)origin.y, (int)(origin.x + accel.x * forceScale), (int)(origin.y + accel.z * forceScale));

	// Draw the historical data
	const uint32 historicalMs = 1000, historicalDrawIntervalMs = 25;
	SDL_Point points[historicalMs / historicalDrawIntervalMs];

	for (uint32 i = game.GetFrameTime() % historicalDrawIntervalMs; i < historicalMs; i += historicalDrawIntervalMs) {
		Vec3 force = game.GetGesture().GetAverageAccel(i + historicalDrawIntervalMs, i);

		points[i / historicalDrawIntervalMs].x = origin.x + force.x * forceScale;
		points[i / historicalDrawIntervalMs].y = origin.y + force.z * forceScale;
	}

	SDL_SetRenderDrawColor(game.GetRenderer(), 255, 0, 255, 255);
	SDL_RenderDrawLines(game.GetRenderer(), points, historicalMs / historicalDrawIntervalMs);
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

	position.y = 700.0f - (gesture.GetAverageAccel(50, 0).z - gesture.GetAverageAccel(1000, 0).z) / 240.0f;

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
