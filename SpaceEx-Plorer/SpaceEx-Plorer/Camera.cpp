#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
	position = glm::vec3(0, 0, -4);

	direction = glm::vec3(0, 0, 0);

	targetRange = 5;

	viewMatrix = glm::lookAt
	(
		position , // Camera is at (0,0,-4), in World Space
		glm::vec3(0, 0, 0),  // And looks at the origin
		glm::vec3(0, -1, 0)  // Head is up ( set to 0,-1,0 to look upside-down )
	);

	speed = 1.0f;
}


Camera::~Camera()
{
}

void Camera::Translate(glm::vec3 &axis)
{
	position += axis * speed;
	target += axis * speed;
}

void Camera::Rotate(float degrees, glm::vec3 &axis)
{
	// Seriously considering not implementing this
}

void Camera::Update()
{
	if (pointToFollow == nullptr)
	{
		std::cout << "oof";
	}
	
	viewMatrix = glm::lookAt
	(
		pointToFollow == nullptr ? position : *pointToFollow + glm::vec3(0, 0, 1) * targetRange, // Camera is at (0,0,-4), in World Space
		pointToFollow == nullptr ? position + glm::vec3(0, 0, 5) : *pointToFollow,  // And looks at the origin
		glm::vec3(0, -1, 0)  // Head is up ( set to 0,-1,0 to look upside-down )
	);
}

void Camera::ResetMatrix()
{
		viewMatrix = glm::lookAt
	(
		position, // Camera is at (0,0,-4), in World Space
		glm::vec3(0, 0, 0),  // And looks at the origin
		glm::vec3(0, -1, 0)  // Head is up ( set to 0,-1,0 to look upside-down )
	);
}
