#include "Types.h"
#include "Game.h"
#include "GameStatePlay.h"

#include "Graph.h"

#include <windows.h> // I didn't wanna do this, but font rendering need.
#include <initializer_list>

// The global variable... KILL IT WITH FIRE! LEST THE STUPID PROGRAMMERS BERAK SOMETHING!!!!
Game game;

const char* port = "COM3";

static HFONT defaultFont;
HDC textSurfaceDc;
HBITMAP textSurfaceBitmap;
uint8* textSurfaceBits; // todo declare variables in the right place
const int textSurfaceSize = 1024;

void Game::Init() {
	AllocConsole();
	freopen("conout$","w",stdout);

	printf("Init SDL\n");
	SDL_Init(SDL_INIT_VIDEO);

	printf("Init windows\n");

	// Create windows and renderers
	SDL_Rect screenBounds;
	SDL_GetDisplayBounds(0, &screenBounds);

	int screenCentreX = screenBounds.w / 2, screenCentreY = screenBounds.h / 2;
	
	struct ScreenInitInfo {RenderScreen screen; const char* windowTitle; int x, y; int width, height;};
	for (const ScreenInitInfo& init : {ScreenInitInfo{Main, "Handzer", -2, -1, 640, 480}, ScreenInitInfo{Debug, "Debug graphs", 0, -1, 600, 600}}) {
		sdlWindows[init.screen] = SDL_CreateWindow(init.windowTitle, screenCentreX + init.x * (init.width / 2), screenCentreY + init.y * (init.height / 2), 
												   init.width, init.height, SDL_WINDOW_RESIZABLE);
	
		if (sdlWindows[init.screen]) {
			sdlRenderers[init.screen] = SDL_CreateRenderer(sdlWindows[init.screen], -1, SDL_RENDERER_ACCELERATED);
			sdlTextSurfaces[init.screen] = SDL_CreateTexture(sdlRenderers[init.screen], SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, textSurfaceSize, textSurfaceSize);
		}
	}

	// Setup renderer hints to make stuff look pretty
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	// Create default font
	printf("Creating additional text rendering assets that nobody wants in their life ever\n");
	defaultFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	// Create text render DC
	textSurfaceDc = CreateCompatibleDC(NULL);

	// Create and select a bitmap into that DC
	int infiniteData[sizeof (BITMAPINFO) + 256 * sizeof (RGBQUAD)]; // lol (needed for bmiColors, prefer not to allocate memory when I can use stack memory)
	BITMAPINFO& bi = *(BITMAPINFO*)infiniteData;
	memset(&bi, 0, sizeof (bi));

	bi.bmiHeader.biSize = sizeof (bi.bmiHeader);
	bi.bmiHeader.biWidth = textSurfaceSize;
	bi.bmiHeader.biHeight = -textSurfaceSize;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 8;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biClrUsed = 256;
	bi.bmiHeader.biClrImportant = 256;
	
	// Give the bitmap a basic monochrome gradient palette
	for (uint32 i = 0; i < 256; ++i) {
		bi.bmiColors[i] = RGBQUAD{(BYTE)i, (BYTE)i, (BYTE)i, 0};
	}

	// Setup GDI objects
	textSurfaceBitmap = CreateDIBSection(0, &bi, DIB_RGB_COLORS, (void**) &textSurfaceBits, nullptr, 0);

	SelectObject(textSurfaceDc, (HGDIOBJ) textSurfaceBitmap);
	SelectObject(textSurfaceDc, (HGDIOBJ) defaultFont);
	SetBkColor(textSurfaceDc, RGB(0, 0, 0));
	SetTextColor(textSurfaceDc, RGB(255, 255, 255));

	// Init sound engine
	sound.Init();

	// Initialise Arduino if possible
	printf("Init Arduino\n");

	arduino = new Serial(port);

	if (arduino->IsConnected())
		printf("Cereal is served!\n");
	else
		printf("Arduino couldn't open. Serial killer on the loose!!\n");

	// Create the debug box
	debugBox = new DebugStringBox(RenderScreen::Main, 0, 0, 500, 500);

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

	// Shutdown dumb WinGDI stuff
	DeleteDC(textSurfaceDc);
	DeleteObject(defaultFont);
	DeleteObject(textSurfaceBitmap);

	// Shutdown SDL components
	for (int i = 0; i < NumRenderScreens; ++i) {
		SDL_DestroyRenderer(sdlRenderers[i]);
		SDL_DestroyWindow(sdlWindows[i]);
		SDL_DestroyTexture(sdlTextSurfaces[i]);
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

	// Now do update the gameplay stuff!
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
}

void Game::Render() {
	// Start rendering
	for (int i = 0; i < RenderScreen::NumRenderScreens; ++i) {
		SDL_SetRenderDrawColor(sdlRenderers[i], 255, 255, 255, 255);
		SDL_RenderClear(sdlRenderers[i]);
	}
	
	// Call current game state's render function
	if (activeGameState) {
		activeGameState->Render();
	}

	// Finish rendering
	for (int i = 0; i < RenderScreen::NumRenderScreens; ++i) {
		SDL_RenderPresent(sdlRenderers[i]);
	}
}

#include "Laser.h"

void Game::Run() {
	// Initialise engine
	Init();

	SetGameState<GameStatePlay>();

	// Spawn the player
	player = SpawnObject<Hand>();

	// Spawn the test laser
	Laser* testLaser = SpawnObject<Laser>();

	// Load the level
	level.Load();

	// Begin main loop!
	SDL_Event sdlEvent;
	uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	uint64 lastPerformanceCounter = SDL_GetPerformanceCounter();


	bool appIsRunning = true;
	startFrameTime = lastPerformanceCounter * 1000 / performanceFrequency;

	while (appIsRunning) {
		// Calculate current frame time
		uint64 currentPerformanceCounter = SDL_GetPerformanceCounter();
		deltaTime = (float) (currentPerformanceCounter - lastPerformanceCounter) / performanceFrequency;
		frameTime = (uint32) (currentPerformanceCounter * 1000 / performanceFrequency) - startFrameTime;

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
			}
		}

		// Update the game!
		Update(deltaTime);

		// Render the game!
		Render();

		// Timer update
		lastPerformanceCounter = currentPerformanceCounter;

		// Reset the debug box
		if (debugBox) {
			debugBox->Reset();
		}
	}

	// Game has ended
	// Unload the level (todo: move)
	level.Unload();

	// Cleanup
	Shutdown();
}

void Game::RenderText(const char* string, int x, int y, RenderScreen screen) {
	// Get text bounds
	RECT r{0, 0, 0, 0};

	DrawTextEx(textSurfaceDc, const_cast<LPSTR>(string), -1, &r, DT_CALCRECT, NULL);

	// Draw the text to the GDI DC
	DrawTextEx(textSurfaceDc, const_cast<LPSTR>(string), -1, &r, 0, NULL);

	GdiFlush();

	// Don't try to render if the size is 0! This causes a crash!
	if (r.right == 0 || r.bottom == 0) {
		return;
	}

	// Copy the GDI text to the SDL texture
	SDL_Rect textureRect{0, 0, r.right, r.bottom};
	uint32* pixels;
	int pitch;
	if (!SDL_LockTexture(sdlTextSurfaces[screen], &textureRect, (void**)&pixels, &pitch)) {
		int textWidth = r.right, textHeight = r.bottom;
		for (int i = 0; i < textHeight; ++i) {
			uint32* pDest = &pixels[i * pitch / 4];
			uint8* pSrc = &textSurfaceBits[i * textSurfaceSize];

			for (int i = 0; i < textWidth; ++i) {
				pDest[i] = pSrc[i] << 24;
			}
		}

		SDL_UnlockTexture(sdlTextSurfaces[screen]);
	}

	if (!SDL_LockTexture(SDL_GetRenderTarget(sdlRenderers[screen]), &textureRect, (void**)&pixels, &pitch)) {
		SDL_UnlockTexture(SDL_GetRenderTarget(sdlRenderers[screen]));
	}

	// Render the SDL texture
	SDL_Rect src = {0, 0, r.right, r.bottom}, dest = {x, y, r.right, r.bottom};

	SDL_SetTextureBlendMode(sdlTextSurfaces[screen], SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(sdlRenderers[screen], sdlTextSurfaces[screen], &src, &dest);
}

void Game::RenderDebugAppurtenances() {
	// Yes I see you there and yes I just wanted an excuse Appurtenances
	for (Object* object : objects) {
		object->RenderCollisionBox();
	}

	level.RenderCollisionBoxes();
}

void Game::ClearObjects() {
	for (Object* obj : objects) {
		delete obj;
	}

	objects.Clear();
}