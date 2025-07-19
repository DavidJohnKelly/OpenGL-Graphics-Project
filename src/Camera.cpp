#include "Camera.h"

Camera::Camera(int width, int height) 
{
	initialise(width, height);
}

void Camera::update() 
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, WORLD_UP);
}

void Camera::handleKeyboard(GLFWwindow* window)
{
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// If shift pressed, speed up movement
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = FAST_SPEED * (float) deltaTime;
	else
		cameraSpeed = DEFAULT_SPEED * (float) deltaTime;

	// WASD based input with space and left control for up and down respectively
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, WORLD_UP)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, WORLD_UP)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cameraPos -= cameraSpeed * WORLD_UP;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += cameraSpeed * WORLD_UP;
}

glm::vec3 Camera::getPosition() const
{
	return cameraPos;
}

glm::mat4 Camera::getView() const
{
	return view;
}

void Camera::initialise(int width, int height)
{
	// Initial all camera values to default starting values
	windowWidth = width;
	windowHeight = height;

	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	direction = glm::vec3(1.0f);

	deltaTime = 0.0f; // Time between current frame and last frame
	lastFrame = 0.0f; // Time of last frame

	lastX = width / 2.0f;
	lastY = height / 2.0f;

	yaw = -90.0f;
	pitch = 0.0f;

	firstClick = true; // Stops erratic jumping of camera

	update();
}

void Camera::handleMouse(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (firstClick)
		{
			glfwSetCursorPos(window, windowWidth / 2.0f, windowHeight / 2.0f);
			x = windowWidth / 2.0f;
			y = windowHeight / 2.0f;
			lastX = x;
			lastY = y;
			firstClick = false;
		}
		
		double xoffset = x - lastX;
		double yoffset = lastY - y; // reversed since y-coordinates range from bottom to top
		lastX = x;
		lastY = y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;		

		direction.x = (float)(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
		direction.y = (float)(sin(glm::radians(pitch)));
		direction.z = (float)(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
		cameraFront = glm::normalize(direction);
	}
	else 
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}
