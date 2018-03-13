#pragma once
#include "Object.h"

class Laser : public Object {
public:
	Laser() {};
	virtual ~Laser();

	void Spawn() override;

	void Update(float deltaTime) override;
	void Render() override;

	void UpdateEffects(float deltaTime);

	Object::Type GetType() const override {
		return LaserType;
	}

private:
	// Primary colour of the laser
	Colour baseColour{Colour::Red()};

	// Average width of the laser, in pixels
	float32 width = 100.0f;

	// Laser power, a scalar between 0 and 1 where 0 is baby alligator and 1 is thermonuclear warhead
	float32 power = 1.0f;

	// All sprite frames
	enum SpriteFrameId {
		CentreFiery = 0,
		CentreRainbow = 1,
		EdgeFlames = 2,
		LightBall = 3,
		NumSpriteFrames,
	};

	// All timers
	float32 nextLightningSpawnTime = 0.0f;

	// Effect list
	Array<class LaserFX*> effects;
};

// Class for special effects spawned by the laser. The laser performs the update and render functions accordingly
class LaserFX {
public:
	virtual void Update(float deltaTime);
	virtual void Render(const Laser* parent) = 0;

	inline float32 GetAge() const;

protected:
	// Effect sprite
	Sprite sprite;

	// Age of the effect, in seconds. Typically determines its destruction time
	float32 age = 0.0f;
};

// Laser-specific effects
class LightningFX : public LaserFX {
public:
	LightningFX() {
		sprite.LoadFrames("graphics/effects/laser/over_lightning", 5, Vec2(77.0f, 133.0f));
		sprite.SetFrameRate(30.0f);

		speed *= Math::randfloat(0.9f, 1.1f);
	}

	void Update(float deltaTime) override;
	void Render(const Laser* parent) override;

private:
	// Speed of the lightning effect in pixels/sec
	float32 speed = 5000.0f;

	// Progress along the laser in pixels
	float32 offset = 0.0f;
};

inline float32 LaserFX::GetAge() const {
	return age;
}