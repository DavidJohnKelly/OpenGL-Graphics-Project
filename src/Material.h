#pragma once

#include "Texture.h"
#include "Model.h"

#include <assimp/Importer.hpp>  
#include <assimp/scene.h>       
#include <assimp/postprocess.h> 
#include <iostream>
#include <fstream>
#include <vector>

class Material
{
public:
	Material(const std::string& path);
	Material(aiMaterial* aiMaterial, std::string& directory);
	~Material();

	std::vector<Texture>& getTextures();
	Texture& getTexture(const char* type);

private:
	void loadMaterial(const std::string& path);

	std::vector<Texture> loadedTextures;
	std::string directory;

	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);
};

