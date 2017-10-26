/*
COMP3320
Project: Trains
Module 1
File name: Camera.h
Students:
Jack Ratcliffe 3236537
Sebastian Brown 3220619
Source: https://github.com/SonarSystems/Modern-OpenGL-Tutorials
Based on the work of http://learnopengl.com/ and http://open.gl/.
*/

#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Object_Movement
{
	F,
	B,
	L,
	R,
};

// Default camera values
const GLfloat OB_YAW = -90.0f;
const GLfloat OB_PITCH = 0.0f;
const GLfloat OB_SPEED = 8.0f;
const GLfloat OB_SENSITIVTY = 0.25f;
const GLfloat OB_ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class MovingGameObject
{
public:

	glm::vec3 position;

	// Constructor with vectors
	MovingGameObject(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = OB_YAW, GLfloat pitch = OB_PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(OB_SPEED), mouseSensitivity(OB_SENSITIVTY), zoom(OB_ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	MovingGameObject(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		glm::mat4 output = glm::lookAt(this->position, this->position + this->front, this->up);
		output = glm::translate(output, glm::vec3(0, 10, 0));
		return output;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Object_Movement direction, GLfloat deltaTime)
	{
		
			GLfloat velocity = this->movementSpeed * deltaTime;

			if (direction == F)
			{
				this->position += this->front * velocity;
			}

			if (direction == B)
			{
				this->position -= this->front * velocity;
			}

			if (direction == L)
			{
				this->position -= this->right * velocity;
			}

			if (direction == R)
			{
				this->position += this->right * velocity;
			}
			

	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		
			xOffset *= this->mouseSensitivity;
			yOffset *= this->mouseSensitivity;

			this->yaw += xOffset;
			this->pitch += yOffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (this->pitch > 89.0f)
				{
					this->pitch = 89.0f;
				}

				if (this->pitch < -89.0f)
				{
					this->pitch = -89.0f;
				}
			}

			// Update Front, Right and Up Vectors using the updated Eular angles
			this->updateCameraVectors();
	}

	glm::mat4 test() {


		
		return glm::lookAt(this->position, this->position + this->front, this->up);

		this->updateCameraVectors();

	}


	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yOffset)
	{
		if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		{
			this->zoom -= yOffset;
		}

		if (this->zoom <= 1.0f)
		{
			this->zoom = 1.0f;
		}

		if (this->zoom >= 45.0f)
		{
			this->zoom = 45.0f;
		}
	}

	GLfloat GetZoom()
	{
		return this->zoom;
	}

	
	

private:
	// Camera Attributes
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;


	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;


	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};