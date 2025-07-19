#pragma once

#include "WindowObject.h"
#include "Light.h"
#include "Model.h"
#include "Plane.h"
#include "Camera.h"
#include "InputManager.h"
#include "RenderManager.h"

#include <vector>

class SceneManager
{
public:
	void initialise(GLFWwindow* window);
	void addObject(WindowObject* object);
	void updateScene(GLFWwindow* window);
	void processMouseClick(GLFWwindow* window, int button, int action, int mods);
	void processMouse(GLFWwindow* window, double xpos, double ypos);

	Camera camera;
	glm::mat4 projection;

private:
	RenderManager renderManager;
	InputManager inputManager;


	std::vector<WindowObject*> WindowObjects;
	std::vector<Light*> LightObjects;
	std::vector<Model*> ModelObjects;
	std::vector<Plane*> PlaneObjects;

	WindowObject* selectedObject = nullptr;
};

