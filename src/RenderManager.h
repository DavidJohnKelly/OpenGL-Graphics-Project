#pragma once

#include "Light.h"
#include "Model.h"
#include "UBO.h"
#include "Camera.h"
#include "Texture.h"
#include "Cubemap.h"
#include "Skybox.h"
#include "Plane.h"

#include <vector>
#include <stb/stb_image.h>

// Class to handle all render functions
// E.g. renders all models and lights etc
// Also updates viewport uniforms for all shaders
class RenderManager
{
public:
	void initialise(GLFWwindow* window, Camera* camera, const glm::mat4* projection);
	void renderSkybox();
	void renderLights(std::vector<Light*>& lights);
	void renderModels(std::vector<Model*>& models);
	void renderPlanes(std::vector<Plane*>& planes);

	void updateMatrixUniforms();

private:
	GLFWwindow* window;
	Camera* camera;
	const glm::mat4* projection;
	
	// UBO used to update all common shader uniforms
	UBO MatricesUBO;
	
	Shader lightingShader;
	Shader lightCubeShader;
	Shader boundingBoxShader;
	Shader skyboxShader;

	int windowWidth;
	int windowHeight;

	Skybox skybox;
};

