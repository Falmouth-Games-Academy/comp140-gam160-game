#pragma once

#include "SObject.h"

// Class for the Camera SObject, only holds information regarding the view matrix
class Camera : public SObject
{
public:
	Camera();
	~Camera();

	glm::mat4 getView() { return viewMatrix; }
	void setView(glm::mat4 inMatrix) { viewMatrix = inMatrix; }

private:
	glm::mat4 viewMatrix;
};

