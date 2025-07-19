#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "WindowObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

// Hold all vertices, indices and textures for a single mesh object
class Mesh
{
public:
	const std::vector<Vertex> vertices;
	const std::vector<GLuint> indices;
	const std::vector<Texture> textures;
	
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);	
	void draw(Shader& shader);
	void remove();

	// Copy constructor to avoid having to load models more than once
	Mesh& operator=(const Mesh& other) {
		if (this != &other) {
			// Copying vertices, indices, and textures
			const_cast<std::vector<Vertex>&>(vertices) = other.vertices;
			const_cast<std::vector<GLuint>&>(indices) = other.indices;
			const_cast<std::vector<Texture>&>(textures) = other.textures;
		}
		return *this;
	}

private:
	VAO vao;
	VBO vbo;
	EBO ebo;
};

