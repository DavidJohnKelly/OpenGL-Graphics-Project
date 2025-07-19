#include "EBO.h"

EBO::EBO()
{
	glGenBuffers(1, &ID);
}

EBO::~EBO()
{
}

void EBO::activate() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::deactivate() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::remove() const
{
	glDeleteBuffers(1, &ID);
}

void EBO::initialiseIndices(const std::vector<GLuint>& indices) const
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void EBO::initialiseIndices(const GLsizei size, const void* data) const
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)size, (GLvoid*)data, GL_STATIC_DRAW);
}

