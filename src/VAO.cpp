#include "VAO.h"

VAO::VAO() 
{
	glGenVertexArrays(1, &ID);
}

VAO::~VAO()
{
}

void VAO::activate() const
{
	glBindVertexArray(ID);
}

void VAO::deactivate() const
{
	glBindVertexArray(0);
}

void VAO::remove() const
{
	glDeleteVertexArrays(1, &ID);
}

void VAO::activateAttribute(const GLuint index, const GLuint size, const GLsizei stride, const GLsizei offset) const
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(intptr_t)offset);
}

void VAO::activateVertexAttributes()
{
	// Encapsulate activation of all vertex attributes
	// Easy to render with this
	int offset = 0;
	activateAttribute(0, 3, sizeof(Vertex), offset);
	offset += sizeof(glm::vec3);
	activateAttribute(1, 3, sizeof(Vertex), offset);
	offset += sizeof(glm::vec3);
	activateAttribute(2, 2, sizeof(Vertex), offset);
	offset += sizeof(glm::vec2);			
	activateAttribute(3, 3, sizeof(Vertex), offset);
	offset += sizeof(glm::vec3);			
}

void VAO::draw(const GLuint count) const
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void VAO::draw(const size_t size) const
{
	glDrawElements(GL_TRIANGLES, (GLuint)size, GL_UNSIGNED_INT, 0);
}