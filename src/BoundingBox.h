#pragma once

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Class used to allow for mouse based object selection
// Objected Oriented and catptures full extent of object vertices
class BoundingBox {

public:
    BoundingBox() {};
    BoundingBox(const glm::vec3& min, const glm::vec3& max);
	void draw(Shader& shader);
    void setMatrices(const glm::mat4* scale, const glm::mat4* rotation, const glm::mat4* translation);
	void setBounds(const glm::vec3& min, const glm::vec3& max);
    void setSelected(bool val);
    glm::vec3 getMinimumVertex();
    glm::vec3 getMaximumVertex();

private:
    // Hold the transformation pointers from the original object
    const glm::mat4* scaleMatrix = nullptr;
    const glm::mat4* rotationMatrix = nullptr;
    const glm::mat4* translationMatrix = nullptr;
    bool selected = false;
    std::vector<Vertex> vertices;
    glm::vec3 maxVertPosition;
    glm::vec3 minVertPosition;

    VAO vao;
    VBO vbo;
    EBO ebo;

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

    void setupModel();
};