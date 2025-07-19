#pragma once

#include "Vertex.h"

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

// Simple OOP class encapsulation for OpenGL VBO
class VBO
{
public:
	GLuint ID;

	VBO();
	~VBO();
	void activate() const;
	void deactivate() const;
	void remove() const;
	void initialiseVertices(const std::vector<Vertex>& vertices) const;
	void initialiseVertices(const GLsizei size, const void* data) const;
};

