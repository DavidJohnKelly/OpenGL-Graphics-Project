#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Class to load and compile an OpenGL shader
class Shader
{
public:
	GLuint ID = -1;

	Shader() {};
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void initialise(const char* vertexPath, const char* fragmentPath);
	void activate() const;

	// Utility functions for uniforms
    void setBool (const std::string& name, const bool       value) const;
    void setInt  (const std::string& name, const int        value) const;
    void setFloat(const std::string& name, const float      value) const;
	void setVec2 (const std::string& name, const glm::vec2& value) const;
	void setVec3 (const std::string& name, const glm::vec3& value) const;
	void setVec4 (const std::string& name, const glm::vec4& value) const;
	void setMat2 (const std::string& name, const glm::mat2& value) const;
	void setMat3 (const std::string& name, const glm::mat3& value) const;
	void setMat4 (const std::string& name, const glm::mat4& value) const;

	GLuint getUniformBlockIndex(const char* name) const;

private:
	std::string loadShaderString(const char* path) const;
	void checkShaderCompile(const GLuint& shader, const GLuint& shaderType);
	void compileShader(GLuint& shader, const GLuint& shaderType, const std::string& shaderCode);
	void checkShaderLink();
	void linkShaders(GLuint& vShader, GLuint& fShader);
};

