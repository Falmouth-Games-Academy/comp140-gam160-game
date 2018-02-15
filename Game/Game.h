#pragma once
#include "SDL.h"
#include "Serial.h"
#include "Containers.h"
#include "Graph.h"
#include "Hand.h"
#include "Gesture.h"
#include "Input.h"
#include "Level.h"

// Main game (singleton class)
// Contains the objects and methods to render and update objects
class Game {
private:
	// Main game flow functions
	void Init();
	void Shutdown();
	
	void Update(float deltaTime);
	void Render();

public:
	// Run the game from start to end!
	void Run();
	
public:
	// Render stuff
	enum RenderScreen : uint8 {
		Main,
		Debug,
		NumRenderScreens
	};

	inline SDL_Renderer* GetRenderer(RenderScreen screen = RenderScreen::Main);
	inline Serial* GetSerialStream(); // Todo: class to handle this
	
	void RenderText(const char* text, int x, int y, RenderScreen screen = RenderScreen::Main); // Renders text. In the game class. Ask no questions just accept fate.
	void RenderRectangle(bool filled = false);

	// Components
	inline GestureManager& GetGesture();
	inline InputManager& GetInput();

	// Time
	inline uint32 GetFrameTime() const;

public:
	// Object stuff
	template<typename ObjectType>
	ObjectType* CreateObject();

	// Da player
	Hand player;

	// Da level
	Level level;

private:
	// Game renderers, one for each window
	SDL_Renderer* sdlRenderers[NumRenderScreens];
	
	// Game windows
	SDL_Window* sdlWindows[NumRenderScreens];
	
	// Surface for text rendering
	SDL_Texture* sdlTextSurfaces[NumRenderScreens];

	// Additional components
	Serial* arduino;
	GestureManager gesture;
	InputManager input;
	
	// Delta time of current frame
	float32 deltaTime;

	// Time of the current frame, in ms, relative to startup
	uint32 frameTime;

	// System time at startup
	uint32 startFrameTime;
};

// The only global variable in the entire game. Justified by the fact that it's the only global variable in the entire game as a rule.
extern Game game;

// Debug string box: Box on the screen containing debug text
class DebugStringBox {
public:
	DebugStringBox(Game::RenderScreen screen_, int x_, int y_, int width_, int height_) : screen(screen_), x(x_), y(y_), width(width_), height(height_), 
			currentTextY(y_) {

		if (SDL_Renderer* renderer = game.GetRenderer(screen)) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

			SDL_Rect rect{x_, y_, width_, height_};
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

	// Draws a string in the box and advances the line pointer
	void DrawString(const char* string) {
		game.RenderText(string, x, currentTextY, screen);
		currentTextY += 20;
	}

private:
	Game::RenderScreen screen;
	int32 currentTextY;
	int32 x, y;
	int32 width, height;
};


/*class GameObject {
	enum Type {
		Ball,
		Paddle
	};

	virtual Type GetType() = 0;
};

class GameScene {
	template <ObjectType>
	ObjectIterator& GetObjects<ObjectType>();

	Array<ObjectList*> objectListsByType;
};*/

// Inlines
inline SDL_Renderer* Game::GetRenderer(RenderScreen screen) {
	return sdlRenderers[screen];
}

inline Serial* Game::GetSerialStream() {
	return arduino;
}

inline GestureManager& Game::GetGesture() {
	return gesture;
}

inline InputManager& Game::GetInput() {
	return input;
}

inline uint32 Game::GetFrameTime() const {
	return frameTime;
}