#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
//#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Defines several possible options for camera movement
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 80.0f;
 

// An abstract Camera class
class Camera {
public:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front, up, right;
	glm::vec3 worldUp;
	// Euler Angles
	float yaw, pitch;
	// Camera options
	float speed;
	float mouseSensitivity;
	float fov;

	// Constructor
	Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 worldUp = glm::vec3(0.0f, 0.0f, 1.0f),
		float speed = SPEED, float fov = FOV, float mouseSensitivity = SENSITIVITY, float yaw = YAW, float pitch = PITCH) {
		
		this->position = position;
		this->front = front;
		this->speed = speed;
		this->fov = fov;
		this->mouseSensitivity = mouseSensitivity;
		this->yaw = yaw;
		this->pitch = pitch;
		this->worldUp = worldUp;
		
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	void processMovement(Camera_Movement direction, float deltaTime) {
		float velocity = speed * deltaTime;
		glm::vec3 forward = glm::normalize(glm::vec3(front.x, front.y, 0));

		if (direction == FORWARD)
			position += forward * velocity;
		if (direction == BACKWARD)
			position -= forward * velocity;
		if (direction == RIGHT)
			position += right * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == UP)
			position += worldUp * velocity;
		if (direction == DOWN)
			position -= worldUp * velocity;
	}

	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
		yaw += xoffset * mouseSensitivity;
		pitch -= yoffset * mouseSensitivity;

		if (constrainPitch) {
			if (pitch > 89.99) pitch = 89.99f;
			if (pitch < -89.99f) pitch = -89.99f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void processMouseScroll(float yoffset) {
		fov -= yoffset;
		if (fov < 1.0f) fov = 1.0f;
		if (fov > 130.0f) fov = 130.0f;

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}


	void print() {
		//COORD o;
		//o.X = 0;
		//o.Y = 0;
		//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), o);
		
		std::cout << "== Camera ==\n"; 
		std::cout << "Position : " << "Vec3(" << position.x << ", " << position.y << ", " << position.z << ")                  \n";
		std::cout << "Front    : " << "Vec3(" << front.x << ", " << front.y << ", " << front.z << ")                  \n";
		std::cout << "Up       : " << "Vec3(" << up.x << ", " << up.y << ", " << up.z << ")                  \n";
		std::cout << "Right    : " << "Vec3(" << right.x << ", " << right.y << ", " << right.z << ")                  \n";
		std::cout << "WorldUp  : " << "Vec3(" << worldUp.x << ", " << worldUp.y << ", " << worldUp.z << ")                  \n";
		std::cout << "Angles   : " << "Yaw(" << yaw << ") Pitch(" << pitch << ")                  \n";
		std::cout << "Speed    : " << speed << " u/s                  \n";
		std::cout << "Sens     : " << mouseSensitivity << "                 \n";
		std::cout << "FOV      : " << fov << "                 \n";
	}

	void updateCameraVectors() {
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.z = sin(glm::radians(pitch));
		front.y = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};