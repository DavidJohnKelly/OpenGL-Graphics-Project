#include "Material.h"

std::string getDirectory(const std::string& path)
{
    std::string directory;
    size_t lastBackslash = path.find_last_of("/");
    if (lastBackslash != std::string::npos) {
        directory = path.substr(0, lastBackslash);
    }
    return directory;
}

Material::Material(const std::string& path)
{
    directory = getDirectory(path);
    std::filesystem::path absolute = std::filesystem::absolute(path);
    loadMaterial(absolute.string());
}

Material::Material(aiMaterial* aiMaterial, std::string& directory)
{
    this->directory = directory;

    loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, TEXTURE_ALBEDO);
    loadMaterialTextures(aiMaterial, aiTextureType_NORMALS, TEXTURE_NORMAL);
    loadMaterialTextures(aiMaterial, aiTextureType_UNKNOWN, TEXTURE_ARM);
}

Material::~Material()
{
}

std::vector<Texture>& Material::getTextures()
{
	return loadedTextures;
}

Texture& Material::getTexture(const char* type)
{
	for (Texture& texture : loadedTextures)
	{
		if (std::strcmp(texture.type.c_str(), type) != 0)
		{
			return texture;
		}
	}
	Texture err;
	std::cout << "Texture type: " << type << " not found" << std::endl;
	return err;
}

void Material::loadMaterial(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    processNode(scene->mRootNode, scene);
}

void Material::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) // Should only be one, just leave for compatiblity
    {
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(aimesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Material::processMesh(aiMesh* mesh, const aiScene* scene)
{
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        loadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_ALBEDO);
        loadMaterialTextures(material, aiTextureType_NORMALS, TEXTURE_NORMAL);
        loadMaterialTextures(material, aiTextureType_UNKNOWN, TEXTURE_ARM);
    }
}

void Material::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;

        for (unsigned int j = 0; j < loadedTextures.size(); j++)
        {
            if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            std::string path = directory + "\\" + std::string(str.C_Str());
            Texture texture(path.c_str(), typeName);
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
}