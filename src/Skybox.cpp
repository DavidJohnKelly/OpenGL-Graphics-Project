#include "Skybox.h"

Skybox::Skybox(std::vector<const char*> faces) : WindowObject()
{
	initialise(faces);
}

Skybox::Skybox(const char* path, const int windowWidth, const int windowHeight) : WindowObject()
{
	initialise(path, windowWidth, windowHeight);
}

void Skybox::draw(Shader& shader)
{
	// Disable depth testing and draw at furthest edge of screen
	glDisable(GL_DEPTH);
	shader.activate();
	glActiveTexture(GL_TEXTURE0);
	cubemap.activate();
	shader.setInt("skybox", 0);
	vao.activate();
	vao.draw(indices.size());
	vao.deactivate();
	glEnable(GL_DEPTH);
}

void Skybox::initialise(std::vector<const char*> faces)
{
	cubemap.initialise(faces);
	setup();
}

void Skybox::initialise(const char* path, const int windowWidth, const int windowHeight)
{
	cubemap.initialise(path, windowWidth, windowHeight);
	setup();
}

void Skybox::setup()
{
	// Setup capture cube
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
