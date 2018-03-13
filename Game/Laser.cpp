#include "Laser.h"
#include "Game.h"

#include "Bottle.h" // Bottle smashing

Laser::~Laser() {
	// Free laser effects
	for (LaserFX* effect : effects) {
		delete effect;
	}

	effects.Clear();
}

void Laser::OnSpawn() {
	// Init angle
	rotation = 0.0f;

	// Load sprite frames
	sprite.LoadFrame("graphics/effects/laser/fiery_centre.png");
	sprite.LoadFrame("graphics/effects/laser/fiery_start.png");
	sprite.LoadFrame("graphics/effects/laser/rainbow_centre.png");
	sprite.LoadFrame("graphics/effects/laser/rainbow_start.png");
	sprite.LoadFrame("graphics/effects/laser/fire_outline_middle.png");
	sprite.LoadFrame("graphics/effects/laser/light_ball.png");

	sprite.GetFrame(StartFiery)->SetBaseOrigin(Vec2(220.0f, 109.0f));
	sprite.GetFrame(CentreFiery)->SetBaseOrigin(Vec2(0.0f, 109.0f));
	sprite.GetFrame(StartRainbow)->SetBaseOrigin(Vec2(248.0f, 109.0f));
	sprite.GetFrame(CentreRainbow)->SetBaseOrigin(Vec2(0.0f, 109.0f));
	sprite.GetFrame(EdgeFlames)->SetBaseOrigin(Vec2(0.0f, 220.0f));
	sprite.GetFrame(LightBall)->SetBaseOrigin(Vec2(100.0f, 100.0f));
}

void Laser::Update(float deltaTime) {
	// Move to player mouth position and angle
	position = game.GetPlayer().SpritePointToWorldPoint(Vec2(1480.0f, 1345.0f));

	rotation = game.GetPlayer().GetRotation();

	if (game.GetPlayer().GetDirection() == -1) {
		rotation += 180.0f;
	}

	// Resize based on player mouth dimensions
	power = game.GetPlayer().GetLaserPower();
	sprite.GetFrame(StartFiery)->SetScale(Vec2(power * 2.0f, power));
	sprite.GetFrame(CentreFiery)->SetScale(Vec2(2000.0f, power));
	sprite.GetFrame(StartRainbow)->SetScale(Vec2(power * 2.0f, power));
	sprite.GetFrame(CentreRainbow)->SetScale(Vec2(2000.0f, power));
	sprite.GetFrame(EdgeFlames)->SetScale(Vec2(2.0f, power));
	sprite.GetFrame(LightBall)->SetScale(Vec2(power * 2.0f, power * 2.0f));

	static float nextCameraShake = 0.0f;

	if (game.GetFrameTime() >= nextCameraShake && power > 0.0f) {
		game.GetCamera().StartShake(0.5f, 40.0f - power * 30.0f, 200.0f * power);;
		nextCameraShake = game.GetFrameTime() + Math::randfloat(0.25f, 0.5f);
	}

	// Destroy any bottles in range of the laser
	if (power >= 0.5f) {
		for (int i = 0; i < game.GetObjects().GetNum(); ++i) {
			Object* obj = game.GetObjects()[i];
			if (obj->GetType() == BottleType) {
				float distance = Vec2::Distance(position.xy, obj->GetPosition().xy);
				float dot = Vec2::Dot(obj->GetPosition().xy - position.xy, Vec2::FromRotation(rotation + 90.0f));

				if (distance >= 600.0f && dot / distance >= 0.95f) {
					((Bottle*)obj)->Smash();
				}
			}
		}
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
	camera.RenderSpriteFrame(sprite, StartFiery, position, rotation + 180.0f);
	camera.RenderSpriteFrame(sprite, CentreFiery, position + Vec2::FromRotation(rotation + 90.0f, sprite.GetFrame(StartFiery)->GetDimensions().x), rotation);

	const float rainbowThreshold = 0.5f;
	if (power >= rainbowThreshold) {
		SDL_SetTextureAlphaMod(sprite.GetFrame(CentreRainbow)->GetSDLTexture(), 255 * ((power - rainbowThreshold) / (1.0f - rainbowThreshold)));
		SDL_SetTextureAlphaMod(sprite.GetFrame(StartRainbow)->GetSDLTexture(), 255 * ((power - rainbowThreshold) / (1.0f - rainbowThreshold)));
		camera.RenderSpriteFrame(sprite, StartRainbow, position + Vec2::FromRotation(rotation + 90.0f, sprite.GetFrame(StartFiery)->GetDimensions().x), rotation, FlipFlags::Horizontal);
		camera.RenderSpriteFrame(sprite, CentreRainbow, position + Vec2::FromRotation(rotation + 90.0f, sprite.GetFrame(StartFiery)->GetDimensions().x), rotation);
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