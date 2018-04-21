#include "Types.h"
#include "Game.h"
#include "GameStatePlay.h"
#include "SDL.h"

#include "Graph.h"

#include <windows.h> // I didn't wanna do this, but font rendering need.
#include <initializer_list>

// The global variable... KILL IT WITH FIRE! LEST THE STUPID PROGRAMMERS BERAK SOMETHING!!!!
Game game;

const char* port = "COM6";

void Game::Init() {
	AllocConsole();
	freopen("conout$", "w", stdout);

	printf("Init SDL\n");
	SDL_Init(SDL_INIT_VIDEO);

	printf("Init windows\n");

	// Create windows and renderers
	SDL_Rect screenBounds;
	SDL_GetDisplayBounds(0, &screenBounds);

	renderers[RenderScreen::Main].Init("Handzer", (screenBounds.w / 2) - 2 * (640 / 2), (screenBounds.h / 2) - 480 / 2, 640, 480);
	renderers[RenderScreen::Debug].Init("Debug graphs", screenBounds.w / 2, (screenBounds.h / 2) - 480 / 2, 640, 480);

	// Enable file drop on window
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	// Setup renderer hints to make stuff look pretty
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	// Init sound engine
	sound.Init();

	// Initialise Arduino if possible
	arduino = new Serial(port);

	if (arduino->IsConnected()) {
		printf("Arduino init successful: Serial is served!\n");
	}
	else {
		printf("Arduino init failed: Serial killer on the loose!\n");
	}

	// Create the debug box
	debugBox = new DebugStringBox(RenderScreen::Main, 0, 0, 500, 500);

	// Done
	printf("Init complete\n");
}

void Game::Shutdown() {
	// Destroy debug box
	delete debugBox;

	// Destroy all game objects
	ClearObjects();

	// Shutdown sound engine
	sound.Shutdown();

	// Shutdown Arduino
	delete arduino;

	// Shutdown renderer
	for (RenderManager& renderer : renderers) {
		renderer.Shutdown();
	}

	SDL_Quit();
}

void Game::Update(float deltaTime) {
	// Update the game state if it's changing
	if (nextGameState) {
		// Switch to the next game state
		if (activeGameState) {
			activeGameState->Exit();
			delete activeGameState;
		}

		activeGameState = nextGameState;
		nextGameState = nullptr;

		// Begin the new state!
		activeGameState->Enter();
	}

	// Now update the gameplay stuff!
	if (activeGameState) {
		// Call current game state's update function
		activeGameState->Update(deltaTime);
	}

	// Update the sound engine
	sound.Update();

	// Destroy any objects pending a kill
	for (int i = 0; i < objects.GetNum(); ++i) {
		if (objects[i]->IsBeingDestroyed()) {
			// Buhbye
			delete objects[i];

			objects.RemoveByIndex(i--);
		}
	}

	// Update debug box password
	if (CheckDebugPassword("debug")) {
		isDebugBoxEnabled = !isDebugBoxEnabled;
	}
}

void Game::Render() {
	// Start rendering
	for (int i = 0; i < RenderScreen::NumRenderScreens; ++i) {
		renderers[i].RenderBegin();
	}
	
	// Call current game state's render function
	if (activeGameState) {
		activeGameState->Render();
	}

	// Render the debug box
	if (debugBox) {
		if (isDebugBoxEnabled) {
			debugBox->Render();
		} else {
			debugBox->ClearStrings();
		}
	}

	// Finish rendering
	for (int i = 0; i < RenderScreen::NumRenderScreens; ++i) {
		renderers[i].RenderEnd();
	}
}

void Game::Run() {
	// Initialise engine
	Init();

	SetGameState<GameStatePlay>();

	// Load the level
	level.Load("./maps/default.hdf");

	// Begin main loop!
	SDL_Event sdlEvent;
	uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	uint64 lastPerformanceCounter = SDL_GetPerformanceCounter();

	bool appIsRunning = true;
	startFrameTime = (uint32) (lastPerformanceCounter * 1000 / performanceFrequency);

	while (appIsRunning) {
		// Calculate current frame time
		const float minFps = 30.0f;
		uint64 currentPerformanceCounter = SDL_GetPerformanceCounter();
		deltaTime = (float) (currentPerformanceCounter - lastPerformanceCounter) / performanceFrequency;
		frameTime = (uint32) (currentPerformanceCounter * 1000 / performanceFrequency) - startFrameTime;

		// Cap delta and frame time
		if (deltaTime > (1.0f / minFps)) {
			deltaTime = 1.0f / minFps;
			frameTime = (int)(1000.0f / minFps);
		}

		// Update the input system before sending it input events--this clears boops
		input.Update();

		// Run SDL event loop
		while (SDL_PollEvent(&sdlEvent)) {
			// Window event
			if (sdlEvent.type == SDL_WINDOWEVENT) {
				// Check if the window has been closed
				if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
					appIsRunning = false;

			// Keyboard or mouse input event
			} else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP || 
					   sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP || 
					   sdlEvent.type == SDL_MOUSEMOTION || sdlEvent.type == SDL_MOUSEWHEEL) {
				// Defer the input event to the input manager
				input.OnInputEvent(sdlEvent);

				// Add the key to the debug password if applicable
				if (sdlEvent.type == SDL_KEYDOWN && (int)sdlEvent.key.keysym.sym >= SDLK_a && (int)sdlEvent.key.keysym.sym <= SDLK_z) {
					for (int i = 1; i < debugPasswordLength; ++i) {
						debugPassword[i - 1] = debugPassword[i];
					}

					debugPassword[debugPasswordLength - 1] = 'a' + (int)sdlEvent.key.keysym.sym - SDLK_a;

					// Triggered
					isDebugPasswordTriggered = true;
				}
			// File drop event
			} else if (sdlEvent.type == SDL_DROPFILE) {
				// Send file path to the input manager
				input.OnFileDropEvent(sdlEvent.drop.file);

				// Cleanup
				SDL_free(sdlEvent.drop.file);
			}
		}

		// Update the game!
		Update(deltaTime);

		// Render the game!
		Render();

		// Timer update
		lastPerformanceCounter = currentPerformanceCounter;

		// Reset debug password trigger
		isDebugPasswordTriggered = false;
	}

	// Game has ended
	// Unload the level (todo: move)
	level.Unload();

	// Cleanup
	Shutdown();
}

void Game::RenderDebugAppurtenances() {
	// Yes I see you there and yes I just wanted an excuse to say Appurtenances
	for (Object* object : objects) {
		object->RenderCollisionBox();
	}

	level.RenderCollisionBoxes();
}

void Game::RespawnPlayer() {
	// Destroy the existing player if necessary
	if (player) {
		player->Destroy();
	}

	// Spawn the player
	player = SpawnObject<Hand>();

	// uh
	player->Respawn();
}

void Game::ClearObjects() {
	// Destroy every object including the player!
	for (Object* obj : objects) {
		delete obj;
	}

	objects.Clear();

	player = nullptr; // todo: dangerous--player should always have a value right?
}

bool Game::CheckDebugPassword(const char* password, bool persistent) {
	if (isDebugPasswordTriggered || persistent) {
		return !strncmp(password, &debugPassword[debugPasswordLength - strlen(password)], strlen(password));
	} else {
		return false;
	}
}