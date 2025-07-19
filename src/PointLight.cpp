#include "PointLight.h"

PointLight::PointLight(
	float x, float y, float z, 
	float strength, glm::vec3 colour
) : Light(x, y, z, strength, colour)
{
	// Update uniform variables
	lightNum = lightCount;
	lightCount++;	
}

void PointLight::emit(Shader& shader)
{
	// Simply update all lighting uniforms
	std::string pointLightIndex = "pointLights[" + std::to_string(lightNum) + "]";

	shader.activate();

	shader.setInt("numPointLights", lightCount);
	shader.setBool(pointLightIndex + ".emit", emitBool);
	if (emitBool) 
	{
		shader.setVec3(pointLightIndex + ".position", translationMatrix[3]);
		shader.setFloat(pointLightIndex + ".strength", strength);
		shader.setVec3(pointLightIndex + ".colour", colour);
	}
}
