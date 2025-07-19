#include "InputManager.h"

void InputManager::initialise(Camera* cam, const glm::mat4* proj)
{
	camera = cam;
	projection = proj;
}

void InputManager::setSelected(WindowObject* object)
{
	if (object)
	{
		selectedObject = object;
	}
}

WindowObject* InputManager::handleMouseClick(GLFWwindow* window, int button, int action, std::vector<WindowObject*>& objects)
{
	// If right click pressed, cast ray and check which object it hit
	// Return the clicked object, or null if none hit
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) 
	{
		glm::vec3 rayWorldOrigin = camera->getPosition();
		glm::vec3 rayWorldDirection = createRayWorldDirection(window);

		WindowObject* clickedObject = processRayIntersections(rayWorldOrigin, rayWorldDirection, objects);
		setSelected(clickedObject);
		return clickedObject;
	}
	return nullptr;
}

void InputManager::handleMouseInput(GLFWwindow* window, double x, double y)
{
	camera->handleMouse(window, x, y);
}

void InputManager::handleKeyboardInput(GLFWwindow* window)
{
	// Either quit if escape pressed, 
	// or allow camera to handle movement, and apply transformations to selected object
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	camera->handleKeyboard(window);
	handleSelectedObject(window, selectedObject);
}

// Implemented with the help of https://antongerdelan.net/opengl/raycasting.html
glm::vec3 InputManager::createRayWorldDirection(GLFWwindow* window)
{
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Get normalised device coordinates
	float x = (2.0f * (float)mouseX) / width - 1.0f;
	float y = 1.0f - (2.0f * (float)mouseY) / height;
	glm::vec3 rayNDC = glm::vec3(x, y, 1.0f);

	// Get homogeneous clip coordinates
	glm::vec4 rayClip = glm::vec4(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

	// Get camera coordinates
	glm::vec4 rayCamera = glm::inverse(*projection) * rayClip;
	rayCamera = glm::vec4(rayCamera.x, rayCamera.y, -1.0f, 0.0f);

	// Get world coordinates
	glm::vec3 rayWorldDirection = glm::mat3(glm::inverse(camera->getView())) * glm::vec3(rayCamera);
	rayWorldDirection = glm::normalize(rayWorldDirection);

	return rayWorldDirection;
}

WindowObject* InputManager::processRayIntersections(glm::vec3& rayWorldOrigin, glm::vec3& rayWorldDirection, std::vector<WindowObject*>& objects)
{
	float closestDistance = std::numeric_limits<float>::infinity();
	WindowObject* closestObject = nullptr;

	for (WindowObject* object : objects)
	{
		float maxEntry, minExit;

		object->getRayEntryExitDistancesBoundingBox(&maxEntry, &minExit, rayWorldOrigin, rayWorldDirection);

		// Check for closest intersection and ignore intersection if camera inside bounding box
		bool intersection = maxEntry < minExit;
		bool closest = minExit < closestDistance;
		bool outside = maxEntry > 0.0f;
		if (intersection && closest && outside)
		{
			closestDistance = minExit;
			closestObject = object;
		}
	}

	if (closestObject)
	{
		for (WindowObject* object : objects)
		{
			object->setSelected(object == closestObject);
		}
		return closestObject;
	}
	return nullptr;
}

void InputManager::handleSelectedObject(GLFWwindow* window, WindowObject* selectedObject)
{
	if (selectedObject)
	{
		// Handle transformation modes
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			selectedObject->setTransformation(Rotate);
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			selectedObject->setTransformation(Translate);
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			selectedObject->setTransformation(Scale);

		// Handle transformation axis
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			selectedObject->setAxis(X);
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			selectedObject->setAxis(Y);
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			selectedObject->setAxis(Z);

		// Handle transformation
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			selectedObject->handleTransformation(Negative);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			selectedObject->handleTransformation(Positive);
	}
}