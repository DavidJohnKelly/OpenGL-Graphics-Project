#pragma once

#include <glm/glm.hpp>

// Simple struct to hold all data required for vertices
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent; // TBN based calculations

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords, const glm::vec3& tangent);
	Vertex(const glm::vec3& position);
	Vertex(const glm::vec3& position, const glm::vec2& texCoords);
	Vertex(const float x, const float y, const float z);
};

