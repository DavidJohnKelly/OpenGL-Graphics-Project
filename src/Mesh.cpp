#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures) :
    vertices(vertices), indices(indices), textures(textures)
{
    // Simply initialise the vao, vbo and ebo to loaded values
    vao.activate();
    vbo.activate();
    ebo.activate();

    vao.activateVertexAttributes();
    vbo.initialiseVertices(vertices);
    ebo.initialiseIndices(indices);

    vao.deactivate();
    vbo.deactivate();
    ebo.deactivate();
}

void Mesh::draw(Shader& shader)
{
    // Draw all mesh textures and draw all vertices
    Texture::drawTextures(shader, textures);

    vao.activate();
    vao.draw(indices.size());
    vao.deactivate();
}

void Mesh::remove()
{
    vao.remove();
    vbo.remove();
    ebo.remove();
}

