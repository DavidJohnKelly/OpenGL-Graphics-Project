#include "VBO.h"

VBO::VBO()
{
	glGenBuffers(1, &ID);
}

VBO::~VBO()
{
}

void VBO::activate() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::deactivate() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::remove() const
{
	glDeleteBuffers(1, &ID);
}

void VBO::initialiseVertices(const std::vector<Vertex>& vertices) const
{
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

void VBO::initialiseVertices(const GLsizei size, const void* data) const
{
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) size, (GLvoid*) data, GL_STATIC_DRAW);
}