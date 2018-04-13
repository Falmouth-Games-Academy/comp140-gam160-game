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
	position = game.GetPlayer().SpritePointToWorldPoint(Vec2(792.0f, 628.0f).Lerped(Vec2(668.0f, 820.0f), power));

	rotation = game.GetPlayer().GetRotation();

	if (game.GetPlayer().GetDirection() == -1) {
		rotation += 180.0f;
	}

	// Resize based on player mouth dimensions
	power = game.GetPlayer().GetLaserPower();
	sprite.GetFrame(StartFiery)->SetScale(Vec2(power * 2.0f, power));
	sprite.GetFrame(CentreFiery)->SetScale(Vec2(5000.0f, power));
	sprite.GetFrame(StartRainbow)->SetScale(Vec2(power * 2.0f * (220.0f / 250.0f), power));
	sprite.GetFrame(CentreRainbow)->SetScale(Vec2(5000.0f, power));
	sprite.GetFrame(EdgeFlames)->SetScale(Vec2(2.0f, power));
	sprite.GetFrame(LightBall)->SetScale(Vec2(power * 2.0f, power * 2.0f));

	// Shake the camera
	static float nextCameraShake = 0.0f;

	if (game.GetFrameTime() >= nextCameraShake && power > 0.0f) {
		game.GetCamera().StartShake(0.5f, 40.0f - power * 30.0f, 100.0f * power);
		nextCameraShake = game.GetFrameTime() + Math::randfloat(0.4f, 0.5f);
	}

	// Update damage on other objects
	UpdateDamage(deltaTime);

	// Update special effects
	UpdateEffects(deltaTime);
}

void Laser::UpdateEffects(float deltaTime) {
	if (power <= 0.0f) {
		// No need to be epic when the laser isn't firing
		return;
	}

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

void Laser::UpdateDamage(float deltaTime) {
	if (!power) {
		// No damage to deal
		return;
	}

	// Damage all objects in range
	Vec2 shootVector = Vec2::FromRotation(rotation + 90.0f);
	float shootDot = Vec2::Dot(shootVector, position.xy);
	Array<Object*>& gameObjects = game.GetObjects();

	for (int i = 0; i < gameObjects.GetNum(); ++i) {
		Object* object = gameObjects[i];

		// Don't attack yourself
		if (object == this || object == &game.GetPlayer()) {
			continue;
		}

		// Don't attack objects behind you
		if (Vec2::Dot(object->GetPosition().xy, shootVector) < shootDot) {
			continue;
		}

		// if (!object->IsInView())
		if (object->GetCollisionFlags() & (Object::OverlapObjs | Object::SolidObjs)) {
			// Check collision using lasy method
			// Check cross of the plane perpendicular to the laser, centering on the middle of the object's collision box
			const Rect2& collisionBox = object->GetCollisionBox();
			Vec3 objectOrigin = object->GetPosition() - object->GetSprite().GetOrigin();
			Vec3 collisionTopLeft = objectOrigin + Vec2(collisionBox.x, collisionBox.y) * object->GetSprite().GetScale();
			Vec3 collisionBottomRight = collisionTopLeft + Vec2(collisionBox.width, collisionBox.height) * object->GetSprite().GetScale();
			Vec3 collisionCentre = (collisionTopLeft + collisionBottomRight) / 2.0f;

			// Find where the laser crosses with the centre of the object
			Vec2 verticalCrossPoint = position.xy + shootVector * (collisionCentre.x - position.x) / Vec2::Dot(Vec2(1.0f, 0.0f), shootVector);
			Vec2 horizontalCrossPoint = position.xy + shootVector * (collisionCentre.y - position.y) / Vec2::Dot(Vec2(0.0f, 1.0f), shootVector);

			// Expand the collision boundaries slightly
			collisionTopLeft -= Vec2(width, width);
			collisionBottomRight += Vec2(width, width);

			// Check if the object crosses the laser
			if (	(horizontalCrossPoint.x >= collisionTopLeft.x && horizontalCrossPoint.x <= collisionBottomRight.x && 
					 horizontalCrossPoint.y >= collisionTopLeft.y && horizontalCrossPoint.y <= collisionBottomRight.y) || 
					(verticalCrossPoint.x >= collisionTopLeft.x && verticalCrossPoint.x <= collisionBottomRight.x && 
					 verticalCrossPoint.y >= collisionTopLeft.y && verticalCrossPoint.y <= collisionBottomRight.y)) {

				// Deal damage to the object
				object->ChangeHealth(-Math::lerpfloat(power, dpsRange) * deltaTime);
			}
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
		SDL_SetTextureAlphaMod(sprite.GetFrame(CentreRainbow)->GetSDLTexture(), (uint8)(255.0f * ((power - rainbowThreshold) / (1.0f - rainbowThreshold))));
		SDL_SetTextureAlphaMod(sprite.GetFrame(StartRainbow)->GetSDLTexture(), (uint8)(255.0f * ((power - rainbowThreshold) / (1.0f - rainbowThreshold))));
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