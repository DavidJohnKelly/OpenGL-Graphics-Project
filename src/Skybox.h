#pragma once

#include "WindowObject.h"
#include "Cubemap.h"

// Class used to get cubemap data in render onto the skybox around the scene
class Skybox : private WindowObject
{
public:
	Skybox() {};
	Skybox(std::vector<const char*> faces);
	Skybox(const char* path, const int windowWidth, const int windowHeight);
	void draw(Shader& shader) override;

	void initialise(std::vector<const char*> faces);
	void initialise(const char* path, const int windowWidth, const int windowHeight);

private:
	static inline const std::vector<Vertex> vertices = 
	{
		 Vertex(0.5f,  0.5f,  0.5f),
		 Vertex(-0.5f,  0.5f,  0.5f),
		 Vertex(-0.5f,  0.5f, -0.5f),
		 Vertex(0.5f,  0.5f, -0.5f),
		 Vertex(0.5f, -0.5f,  0.5f),
		 Vertex(-0.5f, -0.5f,  0.5f),
		 Vertex(-0.5f, -0.5f, -0.5f),
		 Vertex(0.5f, -0.5f, -0.5f)
	};

	static inline const std::vector<GLuint> indices = 
	{
		// Front
		0, 1, 2,
		2, 3, 0,
		// Top
		3, 2, 6,
		6, 7, 3,
		// Back
		7, 6, 5,
		5, 4, 7,
		// Left
		4, 0, 3,
		3, 7, 4,
		// Bottom
		0, 4, 5,
		5, 1, 0,
		// Right
		1, 5, 6,
		6, 2, 1
	};

	Cubemap cubemap;

	void initialiseBounds() override {};
	void setup();
};

