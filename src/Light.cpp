#include "Light.h"

Light::Light(
	float x, float y, float z, 
	float strength, glm::vec3 colour
) : strength(strength), colour(colour)
{
	emitBool = true;
	lightNum = -1;
	visible = false; // Lights invisible by default

	translate(x, y, z);

	setupModel();

	initialiseBounds();
}

void Light::setEmit(bool val) {
	emitBool = val;
}

bool Light::getEmit()
{
	return emitBool;
}

void Light::setStrength(float val)
{
	strength = glm::abs(val);
}

void Light::setColour(glm::vec3 val)
{
	colour = val;
}

void Light::draw(Shader& shader)
{
	// Simply draw the cube representation
	if (visible)
	{
		shader.activate();
		shader.setMat4("model", getModel());
		shader.setVec3("lightColour", colour);

		vao.activate();
		vao.draw(indices.size());
		vao.deactivate();
	}
}

void Light::setupModel() 
{
	// Setup the cube representation
	vao.activate();
	vbo.activate();
	ebo.activate();

	vao.activateAttribute(0, 3, sizeof(Vertex), 0);
	vbo.initialiseVertices(vertices);
	ebo.initialiseIndices(indices);

	vao.deactivate();
	vbo.deactivate();
	ebo.deactivate();
}

void Light::initialiseBounds()
{
	// Simply get bounding box extent from vertices
	glm::vec3 minBounds = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

	for (const Vertex& vertex : vertices)
	{
		minBounds.x = std::min(minBounds.x, vertex.position.x);
		minBounds.y = std::min(minBounds.y, vertex.position.y);
		minBounds.z = std::min(minBounds.z, vertex.position.z);
												   
		maxBounds.x = std::max(maxBounds.x, vertex.position.x);
		maxBounds.y = std::max(maxBounds.y, vertex.position.y);
		maxBounds.z = std::max(maxBounds.z, vertex.position.z);
	}
	
	boundingBox.setBounds(minBounds, maxBounds);
}