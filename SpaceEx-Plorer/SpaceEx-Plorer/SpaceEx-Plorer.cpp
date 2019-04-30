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
#include "SerialInterface.h"

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

const float speedMultiplier = 2.0f;

SerialInterface sInterface;

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

float Remap(float value, float from1, float to1, float from2, float to2)
{
	return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

// Main render function
void Render()
{
	// Tell Renderer to clear everything
	renderer.RenderStart();

	// Render all SObjects
	RenderAllModels();

	unsigned char string[6] = "hello";

	renderer.RenderText(string);

	// Tell our Renderer to swap the buffers
	renderer.RenderEnd();
}

// Update rotation of single model
void UpdateModelRotation(Model* inModel, bool input = false)
{
	glm::vec3 axis = Math::GetRotationAxis(handler);

	//if (axis != glm::vec3(0, 0, 0))
	//{
	//	inModel->Rotate(0.1f, axis);
	//}
	if (input)
	{
		if (std::abs(sInterface.angleX) < 300)
			inModel->Rotate(Remap(sInterface.angleX, 0, 360, 0, 0.05), glm::vec3(1, 0, 0));
		if (std::abs(sInterface.angleY) < 300)
			inModel->Rotate(Remap(sInterface.angleY, 0, 360, 0, 0.05), glm::vec3(0, 1, 0));
		if (std::abs(sInterface.angleZ) < 300)
			inModel->Rotate(Remap(sInterface.angleZ, 0, 360, 0, 0.05), glm::vec3(0, 0, 1));
	}
}

// Upsate single model's position via input
void UpdateModelPosition(Model* inModel, bool input = false, float customSpeed=1.0f)
{
	if (handler.IsKeyDown(SDLK_r))
	{
		inModel->ResetMatrix();
	}
	else
	{
		glm::vec3 axis = glm::vec3(0, 1, 0);

		if (!input)
			inModel->Translate(glm::vec3(0, 1, 0), 0.03f);
		else
			inModel->Translate(axis, customSpeed);

	}

	

	//std::cout << sInterface.speed << " " << sInterface.angleX << " " << sInterface.angleY << " " << sInterface.angleZ << "\n";
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
				std::cout << objects[i].getName() << " is colliding with " << objects[j].getName() << "\n";
				// Check if collision is between a player and an enemy
				if (objects[i].component.type == ComponentType::Player && objects[j].component.type == ComponentType::Enemy)
				{
					objects[i].model.ResetMatrix();
				}
				else if (objects[j].component.type == ComponentType::Player && objects[i].component.type == ComponentType::Enemy)
				{
					objects[j].model.ResetMatrix();
				}
				// Check if collision is between a player and a collectile
			}
			if (isColliding(&objects[i], &objects[j], 2.0f))
			{
				std::cout << objects[i].getName() << " is within 2.0 of " << objects[j].getName() << "\n";
				sInterface.send("P");
			}
			if (!isColliding(&objects[i], &objects[j], 15.0f))
			{
				std::cout << objects[i].getName() << " is too far from " << objects[j].getName() << "\n";
			}
		}
	}
}

float normalisedSpeed(int inSpeed)
{
	inSpeed -= 512;

	return (float)(inSpeed) / 1023 * speedMultiplier;
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

		UpdateModelPosition(&objects[i].model, objects[i].canReceiveInput, normalisedSpeed(sInterface.speed));

		UpdateModelRotation(&objects[i].model, objects[i].canReceiveInput);
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
		// Link player ship receiving input to "handbrake"
		objects[0].canReceiveInput = !sInterface.state;

		// Get input reading from serial
		sInterface.getInput();

		// Update our event handler
		handler.Update();

		// Update our models
		UpdateModels();

		// Update the camera
		UpdateCamera();

		// Check for "collisions"
		CheckForCollisions();

		camera.pointToFollow = &objects[0].getPosition();

		//std::cout << "Camera is at: x = " << camera.getPosition().x << " y = " << camera.getPosition().y << " z = " << camera.getPosition().z << " and  player is at x = " << objects[0].getPosition().x << " y = " << objects[0].getPosition().y << " z = " << objects[0].getPosition().z << std::endl;
		Render();
	}
}
// Create SObject
void spawnSObject(std::string inName,
	std::string inPositionsFile,
	std::string inColoursFile,
	ComponentType type,
	int health = 1,
	int points = 50,
	bool isControlled = false
)
{
	// Create base SObject
	SObject obj = SObject();
	obj.Init(inName, inPositionsFile, inColoursFile, &renderer, isControlled);

	// Add component
	switch (type)
	{
	case ComponentType::Player:
	{
		PlayerComponent pComponent = PlayerComponent();
		pComponent.health = health;
		obj.component = pComponent;

		// Make camera follow the player
		camera.pointToFollow = &obj.getPosition();
		break;
	}
	case ComponentType::Enemy: 
	{
		EnemyComponent eComponent = EnemyComponent();
		eComponent.health = health;
		eComponent.points = points;

		obj.component = eComponent;
		break;
	}
	case ComponentType::Collectible:
	{
		CollectibleComponent cComponent = CollectibleComponent();
		cComponent.health = health;
		cComponent.points = points;

		obj.component = cComponent;
		break;
	}
	}


	// Add to SObject hierarchy
	objects.push_back(obj);
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
	
	PlayerComponent * pComponent = new PlayerComponent();
	pComponent->health = 3;
	objects[0].component = *pComponent;

	//spawnSObject("Player Ship", "positions.txt", "colors.txt", ComponentType::Player, 3, 0, true);
	//spawnSObject("Collectible", "positions_collectible.txt", "colors_collectible.txt", ComponentType::Collectible, 0, 50, false);
	//spawnSObject("Asteroid", "positions_asteroid.txt", "colors_asteroid.txt", ComponentType::Enemy, 0, 50, false);

	camera.pointToFollow = &objects[0].getPosition();
	
	SObject asteroid = SObject();
	asteroid.Init("Asteroid", "positions_asteroid.txt", "colors_asteroid.txt", &renderer, false);
	objects.push_back(asteroid);

	EnemyComponent * eComponent = new EnemyComponent();
	eComponent->health = 1;
	eComponent->points = 50;

	//objects[1].component = *eComponent;

	//SObject collectible = SObject();
	//collectible.Init("Asteroid", "positions_collectible.txt", "colors_collectible.txt", &renderer, false);
	//objects.push_back(asteroid);

	//CollectibleComponent * cComponent = new CollectibleComponent();
	//eComponent->points = 50;

	//objects[2].component = *cComponent;

	//objects[1].target = &objects[0];

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
