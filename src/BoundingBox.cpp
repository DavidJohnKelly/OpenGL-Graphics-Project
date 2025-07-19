#include "BoundingBox.h"

BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
{
    setBounds(min, max);
}

void BoundingBox::draw(Shader& shader)
{
    // Simply get the linked model matrix and draw in the provided shader
    glm::mat4 modelMatrix = (*translationMatrix) * (*rotationMatrix) * (*scaleMatrix);
    shader.activate();
    shader.setMat4("model", modelMatrix);
    shader.setBool("selected", selected);

    vao.activate();
    vao.draw(indices.size());
    vao.deactivate();
}

void BoundingBox::setBounds(const glm::vec3& min, const glm::vec3& max)
{
    // Define the 8 vertices of the bounding box
    vertices.push_back(Vertex(min.x, min.y, max.z));
    vertices.push_back(Vertex(max.x, min.y, max.z));
    vertices.push_back(Vertex(max.x, max.y, max.z));
    vertices.push_back(Vertex(min.x, max.y, max.z));
    vertices.push_back(Vertex(min.x, min.y, min.z));
    vertices.push_back(Vertex(max.x, min.y, min.z));
    vertices.push_back(Vertex(max.x, max.y, min.z));
    vertices.push_back(Vertex(min.x, max.y, min.z));

    // Find the min and max extent of the vertices
    minVertPosition = vertices[0].position;
    maxVertPosition = vertices[0].position;
    for (const Vertex& vertex : vertices)
    {
        minVertPosition = glm::min(minVertPosition, vertex.position);
        maxVertPosition = glm::max(maxVertPosition, vertex.position);
    }

    setupModel();
}

glm::vec3 BoundingBox::getMinimumVertex()
{
    return minVertPosition;
}

glm::vec3 BoundingBox::getMaximumVertex()
{
    return maxVertPosition;
}

void BoundingBox::setupModel()
{
    // Send all required info to the opengl objects
    vao.activate();
    vbo.activate();
    ebo.activate();

    vao.activateAttribute(0, 3, sizeof(Vertex), 0);
    vbo.initialiseVertices(vertices);
    ebo.initialiseIndices(indices);

    vao.deactivate();
    vbo.deactivate();
    ebo.deactivate();
}

void BoundingBox::setMatrices(const glm::mat4* scale, const glm::mat4* rotation, const glm::mat4* translation)
{
    scaleMatrix = scale;
    rotationMatrix = rotation;
    translationMatrix = translation;
}

void BoundingBox::setSelected(bool val)
{
    selected = val;
}