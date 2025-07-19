#pragma once

#include "Light.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Discrete light class for spotlight type
class SpotLight : public Light
{
public:
    SpotLight(float x, float y, float z,
        float strength, glm::vec3 colour, glm::vec3 direction,
        float cutoff, float outerCutoff);

    void emit(Shader& shader) override;
    void rotateX(float degree) override;
    void rotateY(float degree) override;
    void rotateZ(float degree) override;

private:
    static inline unsigned int lightCount = 0;
    glm::vec3 direction;

    float cutoff;
    float outerCutoff;
};
