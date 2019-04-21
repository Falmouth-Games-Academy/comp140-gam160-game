#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
	position = glm::vec3(0, 0, -4);

	direction = glm::vec3(0, 0, 0);

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
	// ugh
}

void Camera::Update()
{
	viewMatrix = glm::lookAt
	(
		position, // Camera is at (0,0,-4), in World Space
		target + direction,  // And looks at the origin
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
