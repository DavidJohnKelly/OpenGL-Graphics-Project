#include "Cubemap.h"

Cubemap::Cubemap()
{
	setup();
}

Cubemap::Cubemap(std::vector<const char*> faces) : Texture()
{
	// In case want to use cubemap without IBL
	setup();
	initialise(faces);
}

Cubemap::Cubemap(const char* path, const int windowWidth, const int windowHeight) : Texture()
{
	// Cubemap and IBL initialisation
	setup();
	initialise(path, windowWidth, windowHeight);
}

void Cubemap::activate() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

// Activate texture storing the data for the IBL diffuse lighting
void Cubemap::activateIrradiance() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceID);
}

void Cubemap::initialise(std::vector<const char*> faces)
{
	// Simply loop through faces and add to cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(faces[i], &width, &height, &num_components, 0);
		if (data)
		{
			GLuint side = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			glTexImage2D(side, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
	}
	applyCubemapTextureOptions();
}

void Cubemap::loadEquirectangularMap(const char* path)
{
	// Load a hdr equirectangular map into memory
	glGenTextures(1, &hdrID);
	glBindTexture(GL_TEXTURE_2D, hdrID);

	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(path, &width, &height, &num_components, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else std::cout << "HDR Cubemap tex failed to load at path: " << path << std::endl;
}

void Cubemap::generateIrradianceMap()
{
	// Generate an irradiance map by blurring the original cubemap
	captureFBO.activate();
	captureRBO.activate();
	captureRBO.assignSize(GL_DEPTH_COMPONENT24, IRRADIANCE_RESOLUTION);

	glGenTextures(1, &irradianceID);
	initialiseCubemapMemory(IRRADIANCE_RESOLUTION, irradianceID);

	applyCubemapTextureOptions();

	irradianceShader.activate();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4("captProjection", captureProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	glViewport(0, 0, IRRADIANCE_RESOLUTION, IRRADIANCE_RESOLUTION);

	captureCubemap(irradianceShader, irradianceID);

	captureFBO.deactivate();
	captureRBO.deactivate();
}

void Cubemap::generateCubemap()
{
	// Cast the equirectangular map to a cube and capture each side, adding to the cubemap
	captureFBO.activate();
	captureRBO.activate();
	captureRBO.assignSize(GL_DEPTH_COMPONENT24, width);
	captureFBO.attatchRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, captureRBO.ID);

	initialiseCubemapMemory(width, ID);

	applyCubemapTextureOptions();

	equirectangularToCubemapShader.activate();
	equirectangularToCubemapShader.setInt("equirectangularMap", 0);
	equirectangularToCubemapShader.setMat4("captProjection", captureProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrID);
	glViewport(0, 0, width, width);

	captureCubemap(equirectangularToCubemapShader, ID);

	captureFBO.deactivate();
	captureRBO.deactivate();
}

void Cubemap::initialise(const char* path, const int windowWidth, const int windowHeight)
{
	loadEquirectangularMap(path);
	glDisable(GL_DEPTH); // Disable depth testing to avoid issues with capturing cube faces
	generateCubemap();
	generateIrradianceMap();
	glEnable(GL_DEPTH);
	glViewport(0, 0, windowWidth, windowHeight);
}

void Cubemap::setup()
{
	// Initialise shaders used to generate cubemaps
	equirectangularToCubemapShader.initialise("cubemap.vert", "cubemap.frag");
	irradianceShader.initialise("cubemap.vert", "cubemapConvolute.frag");

	// Initialise cube used to capture cubemaps
	cubeVAO.activate();
	cubeVBO.activate();
	cubeEBO.activate();
	cubeVAO.activateAttribute(0, 3, sizeof(Vertex), 0);
	cubeVAO.activateAttribute(1, 2, sizeof(Vertex), sizeof(glm::vec3));
	cubeVBO.initialiseVertices(vertices);
	cubeEBO.initialiseIndices(indices);
	cubeVAO.deactivate();
	cubeVBO.deactivate();
	cubeEBO.deactivate();
}

void Cubemap::applyCubemapTextureOptions()
{
	// Simple function to hold all common texture options for cubemap
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Cubemap::initialiseCubemapMemory(unsigned int size, GLuint id)
{
	// Simply initialise enough memory for each face of the cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (unsigned int i = 0; i < 6; i++)
	{
		int side = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glTexImage2D(side, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
	}
}

void Cubemap::captureCubemap(Shader& shader, GLuint texID)
{
	// Simple function to render the texture to the cube and capture each face
	captureFBO.activate();
	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.setMat4("captView", captureViews[i]);
		int side = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, side, texID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		cubeVAO.activate();
		cubeVAO.draw(indices.size());
		cubeVAO.deactivate();
	}
}
