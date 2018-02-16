#pragma once
#include "Math.h"

class Camera {
public:
	void Update();
	void Render();

	const Vec3& GetPosition() const;
	
	void RenderSprite(struct SDL_Texture* texture,
		const Vec3& position, const Vec2& size, 
		float rotation = 0.0f, const Vec2& rotationOrigin = Vec2(0.0f, 0.0f), 
		bool hFlip = false, bool vFlip = false);
	
	const float scalePerZ = 1.0f;

private:
	Vec3 position;

	Rect2 viewBox;
};