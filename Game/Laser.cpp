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
	sprite.LoadFrame("graphics/effects/laser/rainbow_centre.png");
	sprite.LoadFrame("graphics/effects/laser/fire_outline_middle.png");
	sprite.LoadFrame("graphics/effects/laser/light_ball.png");

	sprite.GetFrame(CentreFiery)->SetBaseOrigin(Vec2(0.0f, 109.0f));
	sprite.GetFrame(CentreRainbow)->SetBaseOrigin(Vec2(0.0f, 109.0f));
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
	power = game.GetPlayer().GetLaserPower();
	sprite.GetFrame(CentreFiery)->SetScale(Vec2(2000.0f, power));
	sprite.GetFrame(CentreRainbow)->SetScale(Vec2(2000.0f, power));
	sprite.GetFrame(EdgeFlames)->SetScale(Vec2(2.0f, power));
	sprite.GetFrame(LightBall)->SetScale(Vec2(power, power));

	static float nextCameraShake = 0.0f;

	if (game.GetFrameTime() >= nextCameraShake && power > 0.0f) {
		game.GetCamera().StartShake(0.5f, 40.0f - power * 30.0f, 200.0f * power);;
		nextCameraShake = game.GetFrameTime() + Math::randfloat(0.25f, 0.5f);
	}

	// Test shake camera
	/*static uint32 lastShakeTime = game.GetFrameTime();

	if (game.GetFrameTime() - lastShakeTime >= 2000) {
		//game.GetCamera().StartShake(0.4f, 20.0f, 150.0f);;
		lastShakeTime = game.GetFrameTime();
	}*/

	UpdateEffects(deltaTime);
}

void Laser::UpdateEffects(float deltaTime) {
	// Spawn lightnings occasionally
	if (game.GetFrameTime() >= nextLightningSpawnTime) {
		nextLightningSpawnTime = game.GetFrameTime() + 0.1f + Math::randfloat(-0.05f, 0.05f);

		effects.Append() = new LightningFX();
	}

	// Spawn stars?
	// Spawn fire?
	// Spawn bright flashes?

	// Update special effects
	for (LaserFX* effect : effects) {
		effect->Update(deltaTime);
	}

	// Destroy special effects that have lived too long
	for (int i = 0; i < effects.GetNum(); ++i) {
		if (effects[i]->GetAge() >= 1.0f) {
			delete effects[i];
			effects.RemoveByIndex(i--);
		}
	}
}

void Laser::Render() {
	Camera& camera = game.GetCamera();

	// Render fiery centre
	//camera.RenderSpriteFrame(sprite, EdgeFlames, position, rotation);
	camera.RenderSpriteFrame(sprite, CentreFiery, position, rotation);

	const float rainbowThreshold = 0.5f;
	if (power >= rainbowThreshold) {
		SDL_SetTextureAlphaMod(sprite.GetFrame(CentreRainbow)->GetSDLTexture(), 255 * ((power - rainbowThreshold) / (1.0f - rainbowThreshold)));
		//SDL_SetTextureBlendMode(sprite.GetFrame(CentreRainbow)->GetSDLTexture(), SDL_BLENDMODE_ADD);
		camera.RenderSpriteFrame(sprite, CentreRainbow, position, rotation);
	}

	camera.RenderSpriteFrame(sprite, LightBall, position);

	// Render special effects
	for (LaserFX* effect : effects) {
		effect->Render(this);
	}
}

void LaserFX::Update(float deltaTime) {
	// Update age
	age += deltaTime;
	
	// Update sprite
	sprite.TickAnimation(deltaTime);
}

void LightningFX::Update(float deltaTime) {
	// Super update
	LaserFX::Update(deltaTime);

	// Move the effect
	offset += speed * deltaTime;
}

void LightningFX::Render(const Laser* parent) {
	sprite.SetScale(Vec2(1.0f, parent->GetSprite().GetFrame(0)->GetScale().y));
	game.GetCamera().RenderSprite(sprite, parent->GetPosition() + Vec2::FromRotation(parent->GetRotation() + 90.0f, offset), parent->GetRotation());
}