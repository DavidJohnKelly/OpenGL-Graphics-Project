#pragma once

#include "WindowObject.h"
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Material.h"

#include <glm/glm.hpp>

class Plane : public WindowObject
{
public:
	Plane(float xCent, float yCent, float zCent, float width, float height, Material& material, bool maintainImageScale = false);
	Plane(float xCent, float yCent, float zCent, Material& material, bool maintainImageScale = true);
	Plane(glm::vec3 center, glm::vec2 size, Material& material, bool maintainImageScale = false);
	Plane(glm::vec3 center, Material& material, bool maintainImageScale = true);
	void draw(Shader& shader) override;

	void scale(float scaleX, float scaleY, float scaleZ) override;
	void scale(glm::vec3 scale) override;
private:
	std::vector<Vertex> vertices;
	const std::vector<GLuint> indices = {
		0, 1, 2,
		2, 3, 0
	};
	std::vector<Texture> textures;

	glm::vec2 texScale;
	bool maintainImageScale;

	void initialiseSizedModel(float xCent, float yCent, float zCent, float width, float height, Material& mat);
	void initialiseTextureFitModel(float xCent, float yCent, float zCent, Material& mat);
	void initialiseModel(float xCent, float yCent, float zCent, float width, float height);
	void initialiseTextures(Material& material);
	void initialiseBounds() override;
	void handleScale();
};

