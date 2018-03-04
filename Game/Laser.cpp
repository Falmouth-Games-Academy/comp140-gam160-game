#include "Laser.h"
#include "Game.h"

void Laser::Spawn() {
	// Init angle
	rotation = 0.0f;

	// Load sprites
	spriteFrames[CentreFiery].Load("graphics/effects/laser/fiery_centre.png");
	spriteFrames[EdgeFlames].Load("graphics/effects/laser/fire_outline_middle.png");
	spriteFrames[LightBall].Load("graphics/effects/laser/light_ball.png");

	spriteFrames[OverLightningStart].Load("graphics/effects/laser/over_lightning_0000.png");
	spriteFrames[OverLightningStart+1].Load("graphics/effects/laser/over_lightning_0001.png");
	spriteFrames[OverLightningStart+2].Load("graphics/effects/laser/over_lightning_0002.png");
	spriteFrames[OverLightningStart+3].Load("graphics/effects/laser/over_lightning_0003.png");
	spriteFrames[OverLightningStart+4].Load("graphics/effects/laser/over_lightning_0004.png");
	spriteFrames[OverLightningStart+5].Load("graphics/effects/laser/over_lightning_0005.png");

	spriteFrames[CentreFiery].SetScale(Vec2(500.0f, 1.0f));
	spriteFrames[CentreFiery].SetBaseOrigin(Vec2(0.0f, 109.0f));
}

void Laser::Update(float deltaTime) {
	// Move to player mouth position and angle
	position = game.GetPlayer().GetPosition() - Vec3(0.0f, 150.0f, 0.0f);

	rotation = game.GetPlayer().GetRotation();

	if (game.GetPlayer().GetDirection() == -1) {
		rotation += 180.0f;
	}

	// Test shake camera
	/*static uint32 lastShakeTime = game.GetFrameTime();

	if (game.GetFrameTime() - lastShakeTime >= 2000) {
		//game.GetCamera().StartShake(0.4f, 20.0f, 150.0f);;
		lastShakeTime = game.GetFrameTime();
	}*/

	return;
}

void Laser::Render() {
	Camera& camera = game.GetCamera();

	// Render fiery centre
	camera.RenderSprite(spriteFrames[EdgeFlames], position, rotation);
	camera.RenderSprite(spriteFrames[CentreFiery], position, rotation);
	camera.RenderSprite(spriteFrames[LightBall], position);

	camera.RenderSprite(spriteFrames[OverLightningStart + (game.GetFrameTime()%6)], position);
}