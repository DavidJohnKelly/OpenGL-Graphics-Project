#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "WindowObject.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Material.h"

#include <assimp/Importer.hpp>  
#include <assimp/scene.h>       
#include <assimp/postprocess.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <filesystem>

class Model : public WindowObject
{
public:
	Model(const char* path);
	Model(const Model& model);
	~Model();

	void draw(Shader& shader);
	void initialiseBounds() override;

	Model& operator=(const Model& m)
	{
		// Simply copy mesh vector
		if (this != &m)
		{
			this->meshes = m.meshes;
		}
		// Initialise bounding box based on copied data
		initialiseBounds();
		return *this;
	}

private:
	std::string directory;
	std::vector<Mesh> meshes;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

