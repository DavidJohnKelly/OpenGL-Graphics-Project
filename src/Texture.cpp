#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture()
{
	glGenTextures(1, &ID);
}

Texture::Texture(const char* path, const char* typeName) 
{
	// Use STBI to load image data
	glGenTextures(1, &ID);
	stbi_set_flip_vertically_on_load(true);
	glBindTexture(GL_TEXTURE_2D, ID);
	if (!(std::string(typeName) == TEXTURE_HDR)) // handle general file types
	{
		unsigned char* data = stbi_load(path, &width, &height, &num_components, 0);
		if (data)
		{
			GLenum format{};
			if (num_components == 1)
				format = GL_RED;
			else if (num_components == 3)
				format = GL_RGB;
			else if (num_components == 4)
				format = GL_RGBA;

			// Handle opengl image parameters
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(data);
		}
		else std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	else // handle .hdr images
	{
		float* data = stbi_loadf(path, &width, &height, &num_components, 0); // Floating point for hdr
		if (data)
		{
			// Handle hdr image parameters
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(data);
		}
		else std::cout << ".HDR Texture failed to load at path: " << path << std::endl;
	}
	this->path = std::string(path);
	this->type = std::string(typeName);
}

void Texture::remove() const
{
	glDeleteTextures(1, &ID);
}

void Texture::activate() const
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::drawTextures(Shader& shader, const std::vector<Texture>& textures)
{
	// Simply loop through textures and draw on shader
	shader.activate();
	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i + 1);
		std::string type = textures[i].type;
		shader.setInt(type.c_str(), i + 1);
		textures[i].activate();
	}
	glActiveTexture(GL_TEXTURE0);
}
