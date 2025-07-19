#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glfw/glfw3.h>

constexpr float DEFAULT_SPEED = 2.5f;
constexpr float FAST_SPEED = 4.0f;

constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

// Class for a fly-through camera
class Camera
{
public:
	Camera() {};
	Camera(int width, int height);
	void initialise(int width, int height);
	void update();
	void handleKeyboard(GLFWwindow* window);
	void handleMouse(GLFWwindow* window, double xPos, double yPos);
	glm::vec3 getPosition() const;
	glm::mat4 getView() const;

private:
	int windowWidth;
	int windowHeight;

	float cameraSpeed = DEFAULT_SPEED; // adjust accordingly
	float sensitivity = 0.1f;
	double yaw;
	double pitch;
	bool firstClick;
	
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 direction;
	glm::mat4 view; // Used for MVP calculations

	double deltaTime;
	double lastFrame;
	double lastX;
	double lastY;
};

