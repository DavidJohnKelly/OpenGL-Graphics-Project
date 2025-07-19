#include "UBO.h"

UBO::UBO()
{
	// Initialise empty buffer
	glGenBuffers(1, &ID);
	glBindBuffer(GL_UNIFORM_BUFFER, ID);
	glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UBO::UBO(const GLsizei range)
{
	// Initialise buffer with requested range
	glGenBuffers(1, &ID);
	glBindBuffer(GL_UNIFORM_BUFFER, ID);
	glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ID, 0, range);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UBO::~UBO()
{
	glDeleteBuffers(1, &ID);
}

void UBO::activate() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, ID);
}

void UBO::deactivate() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::bindRange(const GLsizei range) const
{
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ID, 0, range);
}

void UBO::remove() const
{
	glDeleteBuffers(1, &ID);
}

void UBO::activateSubData(const GLsizei offset, const GLsizei size, const glm::f32* data) const
{
	glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)offset, (GLsizeiptr)size, (GLvoid*)data);
}
