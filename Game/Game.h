#pragma once
#include "SDL.h"
#include "Serial.h"
#include "Containers.h"
#include "Graph.h"
#include "Player.h"
#include "Gesture.h"
#include "Input.h"
#include "Level.h"
#include "Camera.h"
#include "GameState.h"
#include "Render.h"
#include "Game.h"
#include "ImageCache.h"
#include "Sound.h"

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
	// Get renderer
	inline SDL_Renderer* GetRenderer(RenderScreen screen = RenderScreen::Main);
	
	// Specialised render stuff. In the game class. Ask no questions, just accept fate.
	void RenderText(const char* text, int x, int y, RenderScreen screen = RenderScreen::Main);
	void RenderRectangle(bool filled = false);

	void RenderDebugAppurtenances();

	// Game states
	template<typename StateType>
	inline void SetGameState();

	// Gameplay objects
	inline Hand& GetPlayer();
	inline Level& GetLevel();

	// Used on level load
	void RespawnPlayer();

	// Components
	inline GestureManager& GetGesture();
	inline InputManager& GetInput();
	inline SoundManager& GetSound();
	inline Camera& GetCamera();
	inline Serial* GetSerialStream(); // Todo: class to handle this
	inline ImageCache& GetImageCache();
	inline class DebugStringBox* GetDebug();

	// Time
	inline float GetFrameTime() const;
	inline uint32 GetFrameTimeMs() const;
	inline float GetDeltaTime() const;

public:
	// World stuff

	// Returns the world's gravity in pixels/sec/sec
	inline constexpr float GetGravity() const;

public:
	// Spawns an object using a template
	template<typename ObjectType>
	ObjectType* SpawnObject();

	// Spawns an object using a type
	Object* SpawnObject(Object::Type objectType);

	// Returns the iterable object list
	inline Array<Object*>& GetObjects();

	// Destroys all objects
	void ClearObjects();

private:
	// Game renderers, one for each window
	SDL_Renderer* sdlRenderers[NumRenderScreens];
	
	// Game windows
	SDL_Window* sdlWindows[NumRenderScreens];
	
	// Surface for text rendering
	SDL_Texture* sdlTextSurfaces[NumRenderScreens];

	// Additional system components
	Serial* arduino;
	GestureManager gesture;
	InputManager input;
	SoundManager sound;
	
	// Currently active gamestate (e.g. GameStatePlay, meaning traditional in-game gameplay)
	GameState* activeGameState;

	// Next gamestate, postponed until the current game state is finished rendering
	GameState* nextGameState;

	// Game objects
	// Da player
	Hand* player;

	// Da level
	Level level;

	// Da camera tho
	Camera camera;

	// The objects
	Array<Object*> objects;

	// The image cache
	ImageCache imageCache;

	// Delta time of current frame
	float32 deltaTime;

	// Time of the current frame, in ms, relative to startup
	uint32 frameTime;

	// System time at startup
	uint32 startFrameTime;

	// Box for debug strings
	DebugStringBox* debugBox;
};

// The only global variable in the entire game. Justified by the fact that it's the only global variable in the entire game as a rule.
extern Game game;

// Debug string box: Box on the screen containing debug text
#include <cassert>

class DebugStringBox {
public:
	DebugStringBox(RenderScreen screen_, int x_, int y_, int width_, int height_) : screen(screen_), x(x_), y(y_), width(width_), height(height_) {
		if (SDL_Renderer* renderer = game.GetRenderer(screen)) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

			SDL_Rect rect{x_, y_, width_, height_};
			//SDL_RenderDrawRect(renderer, &rect);
		}
	}

	~DebugStringBox() {
		ClearStrings();
	}

	// Renders the strings and resets the line pointer
	void Render(bool flushStrings = true) {
		int currentTextY = 0;

		for (char* string : strings) {
			game.RenderText(string, x, y + currentTextY, screen);
			currentTextY += 20;
		}

		if (flushStrings) {
			ClearStrings();
		}
	}

	// Draws a string in the box and advances the line pointer
	void DrawString(const char* string) {
		assert(strings.GetNum() < 30); // Idiot programmer probably forgot to flush the string buffer, who's writing this!?

		const int stringSize = strlen(string) + 1;
		char* deferredString = strings.Append(new char[stringSize]);

		memcpy(deferredString, string, stringSize);
	}

	// Clears the deferred string list
	void ClearStrings() {
		for (char* string : strings) {
			delete[] string;
		}

		strings.Clear();
	}

private:
	RenderScreen screen;

	Array<char*> strings;

	int32 x, y;
	int32 width, height;
};

// Inlines
inline SDL_Renderer* Game::GetRenderer(RenderScreen screen) {
	return sdlRenderers[screen];
}

inline Serial* Game::GetSerialStream() {
	return arduino;
}

inline DebugStringBox* Game::GetDebug() {
	return debugBox;
}

inline GestureManager& Game::GetGesture() {
	return gesture;
}

inline InputManager& Game::GetInput() {
	return input;
}

inline SoundManager& Game::GetSound() {
	return sound;
}

inline Camera& Game::GetCamera() {
	return camera;
};

inline Hand& Game::GetPlayer() {
	return *player;
}

inline Level& Game::GetLevel() {
	return level;
}

inline ImageCache& Game::GetImageCache() {
	return imageCache;
}

inline float Game::GetFrameTime() const {
	return (float)frameTime / 1000.0f;
}

inline uint32 Game::GetFrameTimeMs() const {
	return frameTime;
}

inline float Game::GetDeltaTime() const {
	return deltaTime;
}

inline constexpr float Game::GetGravity() const {
	return 32340.0f;
}

template<typename StateType>
void Game::SetGameState() {
	// If another game state is already pending, overwrite it.......?
	if (nextGameState) {
		// todo: print warning?
		delete nextGameState;
	}

	// Prepare the next game state
	nextGameState = new StateType();
}

template<typename ObjectType>
inline ObjectType* Game::SpawnObject() {
	ObjectType* object = new ObjectType();
	objects.Append(object);

	object->OnSpawn();

	return object;
}

inline Object* Game::SpawnObject(Object::Type objectType) {
	if (Object* object = Object::ConstructByType(objectType)) {
		objects.Append(object);

		object->OnSpawn();

		return object;
	} else {
		return nullptr;
	}
}

inline Array<Object*>& Game::GetObjects() {
	return objects;
}