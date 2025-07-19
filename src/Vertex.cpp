#include "Vertex.h"

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords, const glm::vec3& tangent) :
	position(position), normal(normal), texCoords(texCoords), tangent(tangent) {}

Vertex::Vertex(const glm::vec3& position) :
	position(position), normal(glm::vec3(0.0f)), texCoords(glm::vec2(0.0f)), tangent(glm::vec3(0.0f)) {}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& texCoords) :
	position(position), normal(glm::vec3(0.0f)), texCoords(texCoords), tangent(glm::vec3(0.0f)) {}

Vertex::Vertex(const float x, const float y, const float z) :
	position(glm::vec3(x, y, z)), normal(glm::vec3(0.0f)), texCoords(glm::vec2(0.0f)), tangent(glm::vec3(0.0f)) {}
