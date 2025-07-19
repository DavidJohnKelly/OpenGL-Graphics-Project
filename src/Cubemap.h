#pragma once

#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "FBO.h"
#include "Vertex.h"

#include <stb/stb_image.h>

constexpr unsigned int IRRADIANCE_RESOLUTION = 32;

// Class used from Image Based Diffuse Lighting
// Also used to display the skybox around the scene
struct Cubemap : public Texture
{
	Cubemap();
	Cubemap(std::vector<const char*> faces);
	Cubemap(const char* path, const int windowWidth, const int windowHeight);

	void activate() const override;
    void activateIrradiance() const;
	void initialise(std::vector<const char*> faces);
	void initialise(const char* path, const int windowWidth, const int windowHeight);

private:
    void loadEquirectangularMap(const char* path);
    void applyCubemapTextureOptions();
    void generateIrradianceMap();
    void generateCubemap();
    void setup();

    void initialiseCubemapMemory(unsigned int size, GLuint ID);
    void captureCubemap(Shader& shader, GLuint texID);

private:
	GLuint hdrID, irradianceID;

    FBO captureFBO;
    RBO captureRBO;

    VAO cubeVAO;
    VBO cubeVBO;
    EBO cubeEBO;

	Shader equirectangularToCubemapShader;
    Shader irradianceShader;

    static inline const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f); // Projection matrix to only look at cube
    static inline const glm::mat4 captureViews[6] = // Capture each side of the cube
    {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    static inline const std::vector<Vertex> vertices =
    {
         Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(1.0f, 1.0f)),
         Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 1.0f)),
         Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
         Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
         Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(1.0f, 0.0f)),
         Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f)),
         Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
         Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f))
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
};
