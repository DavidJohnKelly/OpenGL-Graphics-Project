#pragma once

#include "WindowObject.h"
#include "Vertex.h"

#include <vector>
#include <glm/glm.hpp>

// Class to hold all common functionality for the discrete light types
class Light : public WindowObject
{
public:
    Light(
        float x, float y, float z,
        float strength, glm::vec3 colour
    );

    virtual void draw(Shader& shader) override;
    virtual void emit(Shader& shader) = 0;
    virtual void setEmit(bool val);
    virtual bool getEmit();

    virtual void setStrength(float val);
    virtual void setColour(glm::vec3 colour);

protected:
    // Used to update correct uniforms in shader
    unsigned int lightNum;
    bool emitBool;

    float strength;
    glm::vec3 colour;

private:
    void setupModel();
    void initialiseBounds() override;

    static inline const std::vector<Vertex> vertices = {
        Vertex(0.05f,  0.05f,  0.05f),
        Vertex(-0.05f,  0.05f,  0.05f),
        Vertex(-0.05f,  0.05f, -0.05f),
        Vertex(0.05f,  0.05f, -0.05f),
        Vertex(0.05f, -0.05f,  0.05f),
        Vertex(-0.05f, -0.05f,  0.05f),
        Vertex(-0.05f, -0.05f, -0.05f),
        Vertex(0.05f, -0.05f, -0.05f)
    };


    static inline const std::vector<GLuint> indices = {
        // Front
        0, 1, 2, 
        2, 3, 0,
        // Top
        3, 2, 6, 
        6, 7, 3,
        // Back
        7, 6, 5, 
        5, 4, 7,
        // Left
        4, 0, 3, 
        3, 7, 4,
        // Bottom
        0, 4, 5, 
        5, 1, 0,
        // Right
        1, 5, 6, 
        6, 2, 1
    };
};
