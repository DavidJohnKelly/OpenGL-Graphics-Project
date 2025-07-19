#pragma once

#include "WindowObject.h"
#include "Camera.h"

#include <glm/glm.hpp>

// Handle all input requests
// Abstract away from main functions
class InputManager
{
public:
	void initialise(Camera* camera, const glm::mat4* projection);
	void setSelected(WindowObject* object);
	WindowObject* handleMouseClick(GLFWwindow* window, int button, int action, std::vector<WindowObject*>& objects);
	void handleMouseInput(GLFWwindow* window, double x, double y);
	void handleKeyboardInput(GLFWwindow* window);

private:
	Camera* camera;
	const glm::mat4* projection;
 
	WindowObject* selectedObject;

	glm::vec3 createRayWorldDirection(GLFWwindow* window);
	WindowObject* processRayIntersections(glm::vec3& rayWorldOrigin, glm::vec3& rayWorldDirection, std::vector<WindowObject*>& objects);
	void handleSelectedObject(GLFWwindow* window, WindowObject* selectedObject);
};

