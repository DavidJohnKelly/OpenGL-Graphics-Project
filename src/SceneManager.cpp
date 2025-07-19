#include "SceneManager.h"

void SceneManager::initialise(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	camera.initialise(width, height);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	inputManager.initialise(&camera, &projection);
	renderManager.initialise(window, &camera, &projection);
}

void SceneManager::addObject(WindowObject* object)
{
	WindowObjects.push_back(object);

	Light* lightObj = dynamic_cast<Light*>(object);
	Model* modelObj = dynamic_cast<Model*>(object);
	Plane* planeObj = dynamic_cast<Plane*>(object);
	if (lightObj) 
	{
		LightObjects.push_back(lightObj);
	}
	else if (modelObj)
	{
		ModelObjects.push_back(modelObj);
	}
	else if (planeObj)
	{
		PlaneObjects.push_back(planeObj);
	}
}

// Animation Parameters
int trolleyForwardTicks = 0;
int trolleyBackwardTicks = 0;

float vibrationAmplitude = 0.005f;
float vibrationFrequency = 20.0f;

bool doAnimations = true;

void SceneManager::updateScene(GLFWwindow* window)
{
	inputManager.handleKeyboardInput(window);
	camera.update();

	// Control box controls animations
	if (doAnimations)
	{
		// Control trolley movement
		if (trolleyForwardTicks <= 1500)
		{
			ModelObjects[1]->translate(-0.002f, 0.0f, 0.0f);
			trolleyForwardTicks++;
			trolleyBackwardTicks = 0;
		}
		else if (trolleyBackwardTicks <= 1500)
		{
			ModelObjects[1]->translate(0.002f, 0.0f, 0.0f);
			trolleyBackwardTicks++;
		}
		else
		{
			trolleyForwardTicks = 0;
		}

		// Control extractor vibration
		float vibration_angle = vibrationAmplitude * (float)sin(glfwGetTime() * vibrationFrequency);
		ModelObjects[5]->rotateY(vibration_angle);
		ModelObjects[5]->rotateZ(vibration_angle);
	}
	

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderManager.renderModels(ModelObjects);
	renderManager.renderLights(LightObjects);
	renderManager.renderPlanes(PlaneObjects);
	renderManager.renderSkybox();

	renderManager.updateMatrixUniforms();
}

void SceneManager::processMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	WindowObject* previousObject  = selectedObject;
	selectedObject = inputManager.handleMouseClick(window, button, action, WindowObjects);
	if (selectedObject == ModelObjects[0])
	{
		doAnimations = !doAnimations;
	}
}

void SceneManager::processMouse(GLFWwindow* window, double xpos, double ypos)
{
	inputManager.handleMouseInput(window, xpos, ypos);
}

