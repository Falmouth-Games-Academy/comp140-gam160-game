#include "Game.h"
#include "GameStateCalibrate.h"
#include "GameStatePlay.h"

bool GameStateCalibrate::Enter() {
	state = 0;

	// Load sample sprites
	handzerSetup.LoadFrame("graphics/player/calibration/setup1.png");

	handzerOpen.LoadFrame("graphics/player/calibration/open1.png");
	handzerClosed.LoadFrame("graphics/player/calibration/closed1.png");

	return true;
}

void GameStateCalibrate::Exit() {
	return; // wut do?
}

void GameStateCalibrate::Update(float deltaTime) {
	Dimensions2 screenSize = game.GetRenderer().GetScreenSize();

	game.GetGesture().Update();

	// Advance
	if (game.GetInput().IsKeyBooped(SDLK_SPACE)) {
		// Do different calibration behaviours depending on the state
		switch (state) {
			case 0: {
				// Do all of this nothing right here
				break;
			}
			case 1: {
				// Reset orientation and closed angle
				game.GetGesture().SetHandClosedAngle(game.GetGesture().GetFlexAngle());
				game.GetGesture().ResetAccelLocalVectors();
				break;
			}
			case 2: {
				game.GetGesture().SetHandOpenAngle(game.GetGesture().GetFlexAngle());
				break;
			}
		}

		// Increment the state and continue
		++state;

		if (state == numStates) {
			game.SetGameState<GameStatePlay>();
		}
	}
	
	// Lock camera at 0
	game.GetCamera().SetPosition(Vec3(0.0f, 0.0f, 0.0f));

	// Scale images
	handzerSetup.SetScale(Vec2((float)screenSize.width / 1920.0f, (float)screenSize.height / 1080.0f));
	handzerOpen.SetScale(Vec2((float)screenSize.width / 1920.0f, (float)screenSize.height / 1080.0f));
	handzerClosed.SetScale(Vec2((float)screenSize.width / 1920.0f, (float)screenSize.height / 1080.0f));
}


void GameStateCalibrate::Render() {
	Dimensions2 screenSize = game.GetRenderer().GetScreenSize();
	bool textSize = (screenSize.width >= 1280 && screenSize.height >= 720);
	int textHeight = textSize ? 100 : 40;

	switch (state) {
		case 0: {
			int textX = screenSize.width / 2, textY = screenSize.height * 7 / 10 / textHeight;

			// Draw setup text
			game.GetRenderer().RenderText("Hi there! Welcome to Handzer!", textX, (textY++) * textHeight, Colour::Black(), true, textSize);
			game.GetRenderer().RenderText("Fasten the sensors to your right hand as shown", textX, (textY++) * textHeight, Colour::Black(), true, textSize);
			game.GetRenderer().RenderText("When done, press Space", textX, (textY++) * textHeight, Colour::Black(), true, textSize);

			game.GetCamera().RenderSprite(handzerSetup, Vec3((screenSize.width - handzerSetup.GetDimensions().x) * 0.5f, 0.0f, 1.0f));

			break;
		}
		case 1: {
			int textX = screenSize.width / 2, textY = screenSize.height * 7 / 10 / textHeight;

			game.GetRenderer().RenderText("Great! Now move your hand into the position shown", textX, (textY++) * textHeight, Colour::Black(), true, textSize);
			game.GetRenderer().RenderText("While holding your hand in this position, press Space", textX, (textY++) * textHeight, Colour::Black(), true, textSize);
			game.GetRenderer().RenderText("(.....with your other hand)", textX, (textY++) * textHeight, Colour::Black(), true, textSize);

			game.GetCamera().RenderSprite(handzerClosed, Vec3((screenSize.width - handzerClosed.GetDimensions().x) * 0.5f, 0.0f, 1.0f));

			break;
		}
		case 2: {
			int textX = screenSize.width / 2, textY = screenSize.height * 4 / 5 / textHeight;

			game.GetRenderer().RenderText("Nearly there! Now open your hand as wide as you can!", textX, (textY++) * textHeight, Colour::Black(), true, textSize);
			game.GetRenderer().RenderText("While holding this position, press Space to start!", textX, (textY++) * textHeight, Colour::Black(), true, textSize);

			game.GetCamera().RenderSprite(handzerOpen, Vec3((screenSize.width - handzerClosed.GetDimensions().x) * 0.5f, 0.0f, 1.0f));
			break;
		}
	}

	// Render the camera cause we need to
	game.GetCamera().Render();
}