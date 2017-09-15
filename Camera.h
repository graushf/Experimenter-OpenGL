/*
*	This camera system is an FPS-like camera that suits most purposes and works well with Euler Angles,
*	but be careful when creating different camera systems like a flight simulatin camera. Each camera
*	system has its own tricks and quirks so be sure to read up on them. For example, this FPS camera 
*	doesn't allow for pitch values higher than 90 degrees and a static up vector of (0, 1, 0) doesn't 
*	work when we take roll values into account.
*/

#ifndef CAMERA_H
#define CAMERA_H

	// Std. Includes
	#include <vector>
	
	// GL Includes
	#include <GL/glew.h>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	
	// Defines several possible options for camera movement. Used as abstaravtion to stay away from window-system specific input methods
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};


	// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
	class Camera
	{
		public:
			// Camera Attributes
			glm::vec3 Position;
			glm::vec3 Front;
			glm::vec3 Up;
			glm::vec3 Right;
			glm::vec3 WorldUp;
			// Eular angles
			GLfloat Yaw;
			GLfloat Pitch;
			// Camera options
			GLfloat MovementSpeed;
			GLfloat MouseSensitivity;
			GLfloat Zoom;

			// Constructor with vectors
			Camera();
			Camera(glm::vec3 position);
			Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);
			
			// Constructor with scalar values
			Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
			// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
			glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);
			glm::mat4 GetViewMatrix();
			// Processes input received from any keyboard-like input system. Accepts input parameters in the form of camera defined ENUM (to abstract it from windowing systems)
			void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime); // ATTENTION
			// Processes input received from a mouse input system. Expets the offset valye in both the x and y direction.
			void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch);
			// Pocesses input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
			void ProcessMouseScroll(GLfloat yoffset);

		private:
			// Calculates the front vector from the Camera's (updated) Euler Angles
			void updateCameraVectors();
	};


#endif	/* CAMERA_H */