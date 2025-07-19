#include "GLWindow.h"
#include "Model.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SceneManager.h"

#include <iostream>

constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;

GLFWwindow* window = GLWindow(WINDOW_WIDTH, WINDOW_HEIGHT); // Keep at top, GL initialisation occurs here

SceneManager sceneManager;

void processMouse(GLFWwindow* window, double xpos, double ypos)
{
	sceneManager.processMouse(window, xpos, ypos);
}

void processMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	sceneManager.processMouseClick(window, button, action, mods);
}

int main(int argc, int argv[]) 
{
	// FYI the extra libs and dlls are not used by me, they are just dependencies of assimp
	glfwSetCursorPosCallback(window, processMouse);
	glfwSetMouseButtonCallback(window, processMouseClick);

	sceneManager.initialise(window);

	Model control_box("resources/models/Control Box/control_box.gltf");
	control_box.scale(0.5f);
	control_box.translate(2.75f, 1.0f, 2.5f);

	Model trolley("resources/models/Trolley/trolley.gltf");
	trolley.scale(0.5f);
	trolley.translate(5.0f, 0.04f, 0.0f);

	Model furnace("resources/models/Furnace/furnace.gltf");
	furnace.scale(0.85f);
	furnace.translate(0.0f, 0.85f, 0.0f);

	Model pump("resources/models/Pump/pump.gltf");
	pump.scale(0.75f);
	pump.translate(-0.8f, 2.3f, -4.2f);
	pump.rotateY(-0.0f);

	Model tank("resources/models/Tank/tank.gltf");
	tank.scale(0.75f);
	tank.translate(-2.95f, 1.52f, -3.1f);

	Model extractor("resources/models/Extractor/extractor.gltf");
	extractor.scale(0.75f);
	extractor.translate(-0.4f, 0.0f, -0.18f);

	Model tracks("resources/models/Tracks/tracks.gltf");
	tracks.rotateY(90.0f);
	tracks.scale(0.5f);
	tracks.translate(5.0f, -0.21f, 0.0f);

	Model lights("resources/models/Lights/lights.gltf");
	lights.scale(3.0f);
	lights.rotateX(180.0f);
	lights.rotateY(90.0f);
	lights.translate(3.0f, 0.0f, -3.0f);

	Material concrete("resources/materials/Concrete/concrete.gltf");
	Plane floor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(7.5f, 10.0f), concrete);
	floor.rotateY(-90.0f);
	floor.rotateX(90.0f);

	PointLight light1
	(
		2.8f, 0.5f, -1.75f,
		1.0f, glm::vec3(1.0f, 1.0f, 0.9f)
	);
	PointLight light2
	(
		2.8f, 0.5f, -2.75f,
		1.0f, glm::vec3(1.0f, 1.0f, 0.9f)
	);

	PointLight light3
	(
		2.8f, 0.5f, -3.25f,
		1.0f, glm::vec3(1.0f, 1.0f, 0.9f)
	);

	PointLight light4
	(
		2.8f, 0.5f, -4.25f,
		1.0f, glm::vec3(1.0f, 1.0f, 0.9f)
	);

	SpotLight spotLight1
	(
		2.8f, 0.5f, -1.75f,
		15.0f, glm::vec3(1.0f, 1.0f, 0.9f), glm::vec3(-0.6f, 0.4f, 0.45f),
		3.0f, 6.0f
	);

	SpotLight spotLight2
	(
		2.8f, 0.5f, -2.75f,
		15.0f, glm::vec3(1.0f, 1.0f, 0.9f), glm::vec3(-0.6f, 0.4f, -0.45f),
		3.0f, 6.0f
	);

	SpotLight spotLight3
	(
		2.8f, 0.5f, -3.25f,
		15.0f, glm::vec3(1.0f, 1.0f, 0.9f), glm::vec3(-0.6f, 0.4f, 0.45f),
		3.0f, 6.0f
	);

	SpotLight spotLight4
	(
		2.8f, 0.5f, -4.25f,
		15.0f, glm::vec3(1.0f, 1.0f, 0.9f), glm::vec3(-0.6f, 0.4f, 0.45f),
		3.0f, 6.0f
	);

	// Add all models
	sceneManager.addObject(&control_box);
	sceneManager.addObject(&trolley);
	sceneManager.addObject(&furnace);
	sceneManager.addObject(&pump);
	sceneManager.addObject(&tank);
	sceneManager.addObject(&extractor);
	sceneManager.addObject(&tracks);
	sceneManager.addObject(&lights);

	// Add floor plane
	sceneManager.addObject(&floor);

	// Add all lights
	sceneManager.addObject(&light1);
	sceneManager.addObject(&light2);
	sceneManager.addObject(&light3);
	sceneManager.addObject(&light4);
	sceneManager.addObject(&spotLight1);
	sceneManager.addObject(&spotLight2);
	sceneManager.addObject(&spotLight3);
	sceneManager.addObject(&spotLight4);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		sceneManager.updateScene(window);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
