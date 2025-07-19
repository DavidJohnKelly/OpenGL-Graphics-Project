#include "RenderManager.h"

void RenderManager::initialise(GLFWwindow* win, Camera* cam, const glm::mat4* proj)
{
	// Setup all required shaders and uniform values
	camera = cam;
	projection = proj;
	window = win;

	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	lightingShader.initialise("lightingShader.vert", "lightingShader.frag");
	lightCubeShader.initialise("lightCubeShader.vert", "lightCubeShader.frag");
	boundingBoxShader.initialise("boundingBoxShader.vert", "boundingBoxShader.frag");
	skyboxShader.initialise("skyboxShader.vert", "skyboxShader.frag");

	MatricesUBO.activate();
	MatricesUBO.bindRange(2 * sizeof(glm::mat4));
	MatricesUBO.deactivate();

	skybox.initialise("resources/hdris/scythian_tombs_puresky_4k.hdr", windowWidth, windowHeight);
}

void RenderManager::renderSkybox()
{
	// Simply draw skybox
	skybox.draw(skyboxShader);
}

void RenderManager::renderLights(std::vector<Light*>& lights)
{
	// Simply draw all lights
	for (Light* light : lights)
	{
		light->emit(lightingShader);
		light->draw(lightCubeShader);
#ifdef _DEBUG
		light->drawBoundingBox(boundingBoxShader);
#endif
	}
}

void RenderManager::renderModels(std::vector<Model*>& models)
{
	// Simply draw all models
	for (Model* model : models)
	{
		model->draw(lightingShader);
#ifdef _DEBUG
		model->drawBoundingBox(boundingBoxShader);
#endif
	}
}

void RenderManager::renderPlanes(std::vector<Plane*>& planes)
{
	// Simply draw all planes
	for (Plane* plane : planes)
	{
		plane->draw(lightingShader);
#ifdef _DEBUG
		plane->drawBoundingBox(boundingBoxShader);
#endif
	}
}

void RenderManager::updateMatrixUniforms()
{
	// Update common uniforms related to view and position across all shaders
	GLuint UBILighting = lightingShader.getUniformBlockIndex("Matrices");
	GLuint UBILightCubes = lightCubeShader.getUniformBlockIndex("Matrices");
	GLuint UBIBoundingBoxes = boundingBoxShader.getUniformBlockIndex("Matrices");
	GLuint UBISkybox = skyboxShader.getUniformBlockIndex("Matrices");

	glUniformBlockBinding(lightingShader.ID, UBILighting, 0);
	glUniformBlockBinding(lightCubeShader.ID, UBILightCubes, 0);
	glUniformBlockBinding(boundingBoxShader.ID, UBIBoundingBoxes, 0);
	glUniformBlockBinding(skyboxShader.ID, UBISkybox, 0);

	MatricesUBO.activate();
	MatricesUBO.activateSubData(0, sizeof(glm::mat4), glm::value_ptr(*projection));
	MatricesUBO.activateSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->getView()));
	MatricesUBO.deactivate();

	// Update camera position in lighting shader
	lightingShader.activate();
	lightingShader.setVec3("viewPos", camera->getPosition());
}
