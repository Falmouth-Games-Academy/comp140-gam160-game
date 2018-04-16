#pragma once
#include "Serial.h"
#include "Render.h"
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
#include "HUD.h"
#include "DebugStringBox.h"

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
	// Game states
	template<typename StateType>
	inline void SetGameState();

	// Gameplay objects
	inline Hand& GetPlayer();
	inline Level& GetLevel();

	// Components
	inline GestureManager& GetGesture();
	inline InputManager& GetInput();
	inline SoundManager& GetSound();
	inline Camera& GetCamera();
	inline Hud& GetHud();
	inline Serial* GetSerialStream(); // Todo: class to handle this
	inline ImageCache& GetImageCache();
	inline class DebugStringBox* GetDebug();

public:
	// Actions
	// Used on level load
	void RespawnPlayer();

	// Renders debug shtuff
	void RenderDebugAppurtenances();

public:
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

public:
	// Renderer and window stuff
	// Get the renderer for a certain screen
	inline RenderManager& GetRenderer(RenderScreen screen = RenderScreen::Main);

	// Shortcut to get the SDL renderer
	inline SDL_Renderer* GetSDLRenderer(RenderScreen screen = RenderScreen::Main);

private:
	// Additional system components
	Serial* arduino;
	GestureManager gesture;
	InputManager input;
	SoundManager sound;

	// Game renderers, one for each window
	RenderManager renderers[NumRenderScreens];
	
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

	// Da HUD
	Hud hud;

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

// Inlines
inline RenderManager& Game::GetRenderer(RenderScreen screen) {
	return renderers[screen];
}

inline SDL_Renderer* Game::GetSDLRenderer(RenderScreen screen) {
	return renderers[screen].GetSDLRenderer();
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
}

inline Hud& Game::GetHud() {
	return hud;
}

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