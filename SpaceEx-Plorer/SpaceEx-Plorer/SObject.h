#pragma once

// Base class for all in-game objects that can be displayed on-screen

#include "Model.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "SDL.h"
#include "glew.h"

class SObject
{
public:
	// Constructor and destructor
	SObject();
	~SObject();

	// Initialise basic variables
	bool Init(std::string, std::string, std::string, Renderer*, bool);
public:
	// Object 3D representation
	Model model;

	// Perform internal-only operations; input/positioning/rotation is handled in SpaceEx-Plorer.cpp
	void Update();

public:
	// Position (read-only)
	glm::vec3 getPosition() { return position; };

	// Name
	void setName(std::string inString) { name = inString; }
	std::string getName() { return name; }

	bool canReceiveInput = false;

	SObject * target;

	BaseComponent component;

	bool following = false;

private:
	// Read-only
	glm::vec3 position;

	// Read/Writ enabled
	std::string name;
};

