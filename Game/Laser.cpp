#include "Laser.h"
#include "Game.h"

Laser::~Laser() {
	// Free laser effects
	for (LaserFX* effect : effects) {
		delete effect;
	}

	effects.Clear();
}

void Laser::Spawn() {
	// Init angle
	rotation = 0.0f;

	// Load sprite frames
	sprite.LoadFrame("graphics/effects/laser/fiery_centre.png");
	sprite.LoadFrame("graphics/effects/laser/fire_outline_middle.png");
	sprite.LoadFrame("graphics/effects/laser/light_ball.png");

	sprite.GetFrame(CentreFiery)->SetScale(Vec2(500.0f, 1.0f));
	sprite.GetFrame(CentreFiery)->SetBaseOrigin(Vec2(0.0f, 109.0f));
	sprite.GetFrame(EdgeFlames)->SetBaseOrigin(Vec2(0.0f, 220.0f));
	sprite.GetFrame(LightBall)->SetBaseOrigin(Vec2(100.0f, 100.0f));
}

void Laser::Update(float deltaTime) {
	// Move to player mouth position and angle
	position = game.GetPlayer().GetPosition() - Vec3(0.0f, 150.0f, 0.0f);

	rotation = game.GetPlayer().GetRotation();

	if (game.GetPlayer().GetDirection() == -1) {
		rotation += 180.0f;
	}

	// Resize based on player mouth dimensions
	sprite.GetFrame(CentreFiery)->SetScale(Vec2(500.0f, 0.3f));

	// Test shake camera
	/*static uint32 lastShakeTime = game.GetFrameTime();

	if (game.GetFrameTime() - lastShakeTime >= 2000) {
		//game.GetCamera().StartShake(0.4f, 20.0f, 150.0f);;
		lastShakeTime = game.GetFrameTime();
	}*/

	UpdateEffects(deltaTime);

	return;
}

void Laser::UpdateEffects(float deltaTime) {
	if (game.GetFrameTime() >= nextLightningSpawnTime) {
		nextLightningSpawnTime = game.GetFrameTime() + 0.3f + Math::randfloat(-0.1f, 0.1f);

		effects.Append() = new LightningFX();
	}

	// Update special effects
	for (LaserFX* effect : effects) {
		effect->Update(deltaTime);
	}
}

void Laser::Render() {
	Camera& camera = game.GetCamera();

	// Render fiery centre
	camera.RenderSpriteFrame(sprite, EdgeFlames, position, rotation);
	camera.RenderSpriteFrame(sprite, CentreFiery, position, rotation);
	camera.RenderSpriteFrame(sprite, LightBall, position);

	// Render special effects
	for (LaserFX* effect : effects) {
		effect->Render(this);
	}
}

void LightningFX::Update(float deltaTime) {
	offset += speed * deltaTime;

	sprite.TickAnimation(deltaTime);
}

void LightningFX::Render(const Laser* parent) {
	game.GetCamera().RenderSprite(sprite, parent->GetPosition() + Vec2::FromRotation(parent->GetRotation() + 90.0f, offset), parent->GetRotation());
}