#pragma once

#include "Shader.h"
#include "FBO.h"
#include "RBO.h"

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>

// Define all possible texture types
#define TEXTURE_ARM "texture_arm"
#define TEXTURE_ALBEDO "texture_albedo"
#define TEXTURE_ROUGHNESS "texture_roughness"
#define TEXTURE_METAL "texture_metal"
#define TEXTURE_NORMAL "texture_normal"
#define TEXTURE_HDR "texture_hdr"

// Hold all data for the textures loaded through stbi
struct Texture
{
	GLuint ID;
	std::string type;
	std::string path;
	int width;
	int height;
	int num_components;

	Texture();
	Texture(const char* path, const char* typeName);
	void remove() const;
	virtual void activate() const;

	// Static function to render set of textures to a shader
	static void drawTextures(Shader& shader, const std::vector<Texture>& textures);
};

