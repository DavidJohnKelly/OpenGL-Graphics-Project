#include "SpotLight.h"

SpotLight::SpotLight(
    float x, float y, float z, 
    float strength, glm::vec3 colour, glm::vec3 direction,
    float cutoff, float outerCutoff
) : Light(x, y, z, strength, colour),
    direction(direction), cutoff(cutoff), outerCutoff(outerCutoff)
{
    lightNum = lightCount;
    lightCount++;

    // Align model with direction
    glm::vec3 normDirection = glm::normalize(direction);
    float azimuthAngle = glm::degrees(std::atan2(normDirection.x, normDirection.z));
    float pitchAngle = glm::degrees(std::asin(-normDirection.y));
    // Don't need to rotate around Z
    WindowObject::rotateY(azimuthAngle);
    WindowObject::rotateX(pitchAngle);
}

void SpotLight::emit(Shader& shader)
{
    // Simply update shader uniforms
    std::string spotLightIndex = "spotLights[" + std::to_string(lightNum) + "]";

    shader.activate();

    shader.setInt("numSpotLights", lightCount);
    shader.setBool(spotLightIndex + ".emit", emitBool);
    if (emitBool)
    {
        shader.setVec3(spotLightIndex + ".position", translationMatrix[3]);
        shader.setFloat(spotLightIndex + ".strength", strength);
        shader.setVec3(spotLightIndex + ".direction", direction);
        shader.setVec3(spotLightIndex + ".colour", colour);
        shader.setFloat(spotLightIndex + ".cutoff", cutoff);
        shader.setFloat(spotLightIndex + ".outerCutoff", outerCutoff);
    }
}

// Rotate light, and corresponding model

void SpotLight::rotateX(float degree)
{
    glm::mat4 directionRotation = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(1.0f, 0.0f, 0.0f));
    direction = glm::mat3(directionRotation) * direction;

    WindowObject::rotateX(degree);
}

void SpotLight::rotateY(float degree)
{
    glm::mat4 directionRotation = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f));
    direction = glm::mat3(directionRotation) * direction;

    WindowObject::rotateY(degree);
}

void SpotLight::rotateZ(float degree)
{
    glm::mat4 directionRotation = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
    direction = glm::mat3(directionRotation) * direction;

    WindowObject::rotateZ(degree);
}
