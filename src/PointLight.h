#pragma once

#include "Light.h"

// Discrete light class for point light
class PointLight :
    public Light
{
public:
    PointLight(
        float x, float y, float z, 
        float strength, glm::vec3 colour
    );

    void emit(Shader& shader) override;
    
private:
    // Hold all pointlight number count
    static inline unsigned int lightCount = 0;
};
