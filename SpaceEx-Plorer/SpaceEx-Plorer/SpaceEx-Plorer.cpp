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

// Handles all our SDL2 event ( quit, keydown, ++ )
EventHandler handler;

// Handles everything related to OpenGL
Renderer renderer;

//// Handles a single model with its vertexes and matrix
//Model model;

std::vector<SObject> objects;

void PrintCommands();
void UpdateModelPosition(Model);
void UpdateModelRotation(Model);
void UpdateCamera();

void RenderSingleModel(Model inModel)
{
	// Set the matrix in the model
	// This has to be done once per model
	renderer.SetMatrix(inModel.GetModel());

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

// Update all SObject's positions and rotations based on velocities and inputs
void UpdateModels()
{
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
	//renderer.UpdateCamera(model.GetCenter());
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

		UpdateCamera();

		Render();
	}
}

int main(int argc, char *argv[])
{
	if (!renderer.Init())
		return -1;

	PrintCommands();

	// Set up our only shader in Shader
	renderer.SetUpShader("vert.glsl", "frag.glsl");

	SObject playerShip = SObject();
	playerShip.Init("Player Ship", "positions.txt", "colors.txt", &renderer, false);
	objects.push_back(playerShip);

	SObject asteroid = SObject();
	asteroid.Init("Asteroid", "positions_asteroid.txt", "colors_asteroid.txt", &renderer, true);
	objects.push_back(asteroid);

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
