#include "Plane.h"

Plane::Plane(float xCent, float yCent, float zCent, float width, float height, Material& material, bool imgScale)
{
	maintainImageScale = imgScale;
	initialiseSizedModel(xCent, yCent, zCent, width, height, material);
	initialiseTextures(material);
}

Plane::Plane(float xCent, float yCent, float zCent, Material& material, bool imgScale)
{
	maintainImageScale = imgScale;
	initialiseTextureFitModel(xCent, yCent, zCent, material);
	initialiseTextures(material);
}

Plane::Plane(glm::vec3 center, glm::vec2 size, Material& material, bool imgScale)
{
	maintainImageScale = imgScale;
	initialiseSizedModel(center.x, center.y, center.z, size.x, size.y, material);
	initialiseTextures(material);
}

Plane::Plane(glm::vec3 center, Material& material, bool imgScale)
{
	maintainImageScale = imgScale;
	initialiseTextureFitModel(center.x, center.y, center.z, material);
	initialiseTextures(material);
}

void Plane::draw(Shader& shader)
{
	if (visible)
	{
		shader.activate();
		shader.setMat4("model", getModel());

		Texture::drawTextures(shader, textures);

		vao.activate();
		vao.draw(indices.size());
		vao.deactivate();
	}
}

void Plane::scale(float scaleX, float scaleY, float scaleZ)
{
	if (!maintainImageScale) {
		texScale.x *= scaleX;
		texScale.y *= scaleY;

		handleScale();
	}

	WindowObject::scale(scaleX, scaleY, 1.0f);
}

void Plane::scale(glm::vec3 scale)
{
	if (!maintainImageScale) {
		texScale.x *= scale.x;
		texScale.y *= scale.y;

		handleScale();
	}

	WindowObject::scale(scale.x, scale.y, 1.0f);
}

void Plane::initialiseSizedModel(float xCent, float yCent, float zCent, float width, float height, Material& material)
{
	Texture& texture = material.getTextures()[0];
	texScale.x = width * (float)texture.width / (float)texture.height;
	texScale.y = height * (float)texture.width / (float)texture.height;
	initialiseModel(xCent, yCent, zCent, width, height);
}

void Plane::initialiseTextureFitModel(float xCent, float yCent, float zCent, Material& material)
{
	Texture& texture = material.getTextures()[0];
	float ratio = (float)texture.width / (float)texture.height;
	float width = (ratio * texture.width) / 200.f;
	float height = (ratio * texture.height) / 200.f;
	texScale = glm::vec2(1.0f, 1.0f);
	initialiseModel(xCent, yCent, zCent, width, height);
}

void Plane::initialiseModel(float xCent, float yCent, float zCent, float width, float height)
{
	glm::vec3 pos1(-width, height, 0.0f);
	glm::vec3 pos2(width, height, 0.0f);
	glm::vec3 pos3(width, -height, 0.0f);
	glm::vec3 pos4(-width, -height, 0.0f);

	glm::vec2 uv1(0.0f, texScale.y);
	glm::vec2 uv2(texScale.x, texScale.y);
	glm::vec2 uv3(texScale.x, 0.0f);
	glm::vec2 uv4(0.0f, 0.0f);

	glm::vec3 normal(0.0f, 0.0f, -1.0f); // Plane initialised in x, y planes so normal in z plane

	glm::vec3 tangent = glm::normalize(glm::cross(normal, glm::vec3(1.0f, 0.0f, 0.0f)));

	vertices.push_back(Vertex(pos1, normal, uv1, tangent)); // Top Left
	vertices.push_back(Vertex(pos2, normal, uv2, tangent)); // Top Right
	vertices.push_back(Vertex(pos3, normal, uv3, tangent)); // Bottom Right
	vertices.push_back(Vertex(pos4, normal, uv4, tangent)); // Bottom Left

	translate(xCent, yCent, zCent);
	
	vao.activate();
	vbo.activate();
	ebo.activate();

	vao.activateVertexAttributes();
	vbo.initialiseVertices(vertices);
	ebo.initialiseIndices(indices);

	vao.deactivate();
	vbo.deactivate();
	ebo.deactivate();

	initialiseBounds();
}

void Plane::initialiseTextures(Material& material)
{
	for (const Texture& texture : material.getTextures())
	{
		textures.push_back(texture);
	}
}

void Plane::initialiseBounds()
{
	glm::vec3 minBounds = vertices[0].position;
	glm::vec3 maxBounds = vertices[0].position;

	for (const Vertex& vertex : vertices)
	{
		minBounds.x = std::min(minBounds.x, vertex.position.x);
		minBounds.y = std::min(minBounds.y, vertex.position.y);
		minBounds.z = std::min(minBounds.z, vertex.position.z);

		maxBounds.x = std::max(maxBounds.x, vertex.position.x);
		maxBounds.y = std::max(maxBounds.y, vertex.position.y);
		maxBounds.z = std::max(maxBounds.z, vertex.position.z);
	}

	minBounds.z -= 0.01f;
	maxBounds.z += 0.01f;

	boundingBox.setBounds(minBounds, maxBounds);
}

void Plane::handleScale()
{
	vertices[0].texCoords = glm::vec2(0.0f, texScale.y);
	vertices[1].texCoords = glm::vec2(texScale.x, texScale.y);
	vertices[2].texCoords = glm::vec2(texScale.x, 0.0f);
	vertices[3].texCoords = glm::vec2(0.0f, 0.0f);

	vbo.activate();
	vbo.initialiseVertices(vertices);
	vbo.deactivate();
}
