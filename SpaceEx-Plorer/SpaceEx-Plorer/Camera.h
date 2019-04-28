#pragma once

#include <SDL.h>
#include <glew.h>
#include <glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Shader.h"

// Class for the Camera SObject, only holds information regarding the view matrix
class Camera 
{
public:
	Camera();
	~Camera();

	// Viewmatrix getters and setters
	glm::mat4 getView() { return viewMatrix; }
	void setView(glm::mat4 inMatrix) { viewMatrix = inMatrix; }

	// Standard manipulation
	void Translate(glm::vec3 &);
	void Rotate(float, glm::vec3 &);

	// Update view matrix every frame
	void Update();

	// Reset to default matrix
	void ResetMatrix();

	// Camera movement speed
	float speed;

	// Distance to view target
	float targetRange;

	glm::vec3 getPosition() { return position; }

	glm::vec3 * pointToFollow;
private:
	glm::mat4 viewMatrix;

	glm::vec3 direction;

	glm::vec3 position;
	glm::vec3 target;

	glm::vec3 up;
};

