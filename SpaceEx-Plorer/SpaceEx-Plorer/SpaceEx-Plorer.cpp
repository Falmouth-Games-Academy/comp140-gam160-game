// headerphile.com - main.cpp
// This is the main manager the class that controls everything
// It doesn't do much on it's own, it just tells the other classes do to stuff
//
// Compile : clang++ main.cpp EventHandler.cpp -o Part_04 -lGL -lSDL2 -lGLEW -lstd=c++11
//
#include "pch.h"

#include <iostream>

#include "Renderer.h"
#include "EventHandler.h"
#include "Math.h"
#include "SObject.h"
#include "Camera.h"

// Handles all our SDL2 event ( quit, keydown, ++ )
EventHandler handler;

// Handles everything related to OpenGL
Renderer renderer;

// Acts as a container class for teh view matrix
Camera camera;

//// Handles a single model with its vertexes and matrix
//Model model;

SDL_Window* mainWindow;

SDL_Surface* windowSurface;

std::vector<SObject> objects;

void PrintCommands();
void UpdateModelPosition(Model);
void UpdateModelRotation(Model);
void UpdateCamera();

void RenderSingleModel(Model inModel)
{
	// Set the matrix in the model
	// This has to be done once per model
	renderer.SetMatrix(inModel.GetModel(), camera);

	// Render our model
	renderer.RenderModel(inModel);
}

// Render all the SObjects' models
void RenderAllModels()
{	
	for (int i = 0; i < objects.size(); i++)
	{
		RenderSingleModel(objects[i].model);
	}
}

// Main render function
void Render()
{
	// Tell Renderer to clear everything
	renderer.RenderStart();

	// Render all SObjects
	RenderAllModels();

	// Tell our Renderer to swap the buffers
	renderer.RenderEnd();
}

// Update rotation of single model
void UpdateModelRotation(Model* inModel)
{
	glm::vec3 axis = Math::GetRotationAxis(handler);

	if (axis != glm::vec3(0, 0, 0))
		inModel->Rotate(0.1f, axis);
}

// Upsate single model's position via input
void UpdateModelPosition(Model* inModel)
{
	if (handler.IsKeyDown(SDLK_r))
	{
		inModel->ResetMatrix();
	}
	else
	{
		glm::vec3 axis = Math::GetAxis(handler);

		inModel->Translate(axis);
	}
}

// Return true if A is close enough to B - pseudo collision detection
bool isColliding(SObject *A, SObject *B, float colRange=1.0f)
{
	return glm::length(A->getPosition() - B->getPosition()) < colRange ? true : false;
}

// Check for collisions within the SOBjects array
void CheckForCollisions()
{
	for (int i = 0; i < objects.size() - 1; i++)
	{
		for (int j = i + 1; j < objects.size(); j++)
		{
			if (isColliding(&objects[i], &objects[j]))
			{
				//std::cout << objects[i].getName() << " is colliding with " << objects[j].getName() << "\n";
			}
		}
	}
}

// Update all SObject's positions and rotations based on velocities and inputs
void UpdateModels()
{
	if (handler.IsKeyDown(SDLK_f))
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i].target != nullptr)
			{
				objects[i].following = !objects[i].following;
			}
		}
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].Update();

		// Handle translations for controlled objects
		if (objects[i].canReceiveInput)
		{
			UpdateModelPosition(&objects[i].model);

			UpdateModelRotation(&objects[i].model);
		}
	}
}
void UpdateCamera()
{
	if (handler.IsKeyDown(SDLK_r))
	{
		camera.ResetMatrix();
	}
	else
	{
		glm::vec3 posAxis = Math::GetAxis(handler);
		glm::vec3 rotAxis = Math::GetRotationAxis(handler);

		camera.Translate(posAxis);	// Apparently redundant???? i.e. camera translates without this line
		camera.Rotate(2, rotAxis);	// Currently redundant
	}

	camera.Update();
}

// Main game loop
void Update()
{
	while (!handler.WasQuit())
	{
		// Update our event handler
		handler.Update();

		// Update our models
		UpdateModels();

		// Update the camera
		UpdateCamera();

		// Check for "collisions"
		CheckForCollisions();

		Render();
	}
}

int main(int argc, char *argv[])
{
	if (!renderer.Init())
		return -1;

	camera = Camera();

	PrintCommands();

	// Set up our only shader in Shader
	renderer.SetUpShader("vert.glsl", "frag.glsl");

	SObject playerShip = SObject();
	playerShip.Init("Player Ship", "positions.txt", "colors.txt", &renderer, true);
	objects.push_back(playerShip);

	SObject asteroid = SObject();
	asteroid.Init("Asteroid", "positions_asteroid.txt", "colors_asteroid.txt", &renderer, false);
	objects.push_back(asteroid);

	objects[1].target = &objects[0];

	// Set up the main window
	mainWindow = renderer.getWindow();

	windowSurface = SDL_GetWindowSurface(mainWindow);

	// Run our main update loop
	Update();

	renderer.Cleanup();

	return 0;
}

void PrintCommands()
{
	std::cout << "Controls :"
		<< "\n\tMove left : \t\t\t Left"
		<< "\n\tMove right : \t\t\t Right"
		<< "\n\tMove up : \t\t\t Up"
		<< "\n\tMove down : \t\t\t Down"
		<< "\n\tMove clockwise : \t\t w"
		<< "\n\tMove counter-clockwise : \t s"
		<< std::endl;
}
