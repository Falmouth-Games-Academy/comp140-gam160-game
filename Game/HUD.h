#pragma once

// Isn't this the cutest widdle cwass you've ever seen!
class Hud {
public:
	void Render();

private:
	// Dimensions of the healthbar, as fractions of the screen size
	Rect2 healthbarRect = Rect2(0.3f, 0.9f, 0.4f, 0.05f);
};