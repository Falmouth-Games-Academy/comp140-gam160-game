#include "Game.h"

void Hud::Render() {
	SDL_Renderer* renderer = game.GetSDLRenderer();

	// Draw healthbar
	float32 health = game.GetPlayer().GetHealth();
	const float32 maxHealth = 100.0f;
	const float32 criticalHealthPoint = maxHealth * 0.2f;

	const Colour healthColour = Colour::Green();
	const Colour criticalHealthColour = Colour::Red();

	Dimensions2 screenSize = game.GetRenderer(Main).GetScreenSize();
	SDL_Rect sdlHealthRect = {(int)(healthbarRect.x * (float)screenSize.width), (int)(healthbarRect.y * (float)screenSize.height), 
							  (int)(healthbarRect.width * (float)screenSize.width), (int)(healthbarRect.height * (float)screenSize.height)};

	// Draw the background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &sdlHealthRect);

	// Draw the bar
	sdlHealthRect.w = (int)((float)sdlHealthRect.w * health / maxHealth);

	if (health > criticalHealthPoint) {
		Colour barColour = healthColour.Lerped(criticalHealthColour, 1.0f - (health - criticalHealthPoint) / (maxHealth - criticalHealthPoint));

		SDL_SetRenderDrawColor(renderer, barColour.r, barColour.g, barColour.b, barColour.a);
	} else {
		// Draw flashing healthbar
		const float32 flashRate = 0.1f;

		if (Math::circleclamp(game.GetFrameTime(), flashRate * 2.0f) <= flashRate) {
			SDL_SetRenderDrawColor(renderer, healthColour.r, healthColour.g, healthColour.b, healthColour.a);
		} else {
			SDL_SetRenderDrawColor(renderer, criticalHealthColour.r, criticalHealthColour.g, criticalHealthColour.b, criticalHealthColour.a);
		}
	}

	SDL_RenderFillRect(renderer, &sdlHealthRect);
}
