#include "Shader.h"

#define SHADER_UNIFORM_INFO // Whether to output detailed uniform info

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	initialise(vertexPath, fragmentPath);
}

Shader::~Shader()
{
	glDeleteShader(ID);
}

void Shader::initialise(const char* vertexPath, const char* fragmentPath)
{
	// Compile and link vertex and fragment code
	std::string vertexCode = loadShaderString(vertexPath);
	std::string fragmentCode = loadShaderString(fragmentPath);

	GLuint vertex, fragment;

	compileShader(vertex, GL_VERTEX_SHADER, vertexCode);
	compileShader(fragment, GL_FRAGMENT_SHADER, fragmentCode);

	linkShaders(vertex, fragment);
}

std::string Shader::loadShaderString(const char* path) const
{
	// Simply load the raw text from the shader file
	try
	{
		std::ifstream shaderFile(path);

		if (!shaderFile.is_open())
			throw std::ifstream::failure("File not found");
		
		return std::string(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());
	}
	catch (const std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ " << path << std::endl;
		std::cout << e.what() << std::endl;
		return {};
	}
}


void Shader::checkShaderCompile(const GLuint& shader, const GLuint& shaderType)
{
	// Check for compilation sucess
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		if (shaderType == GL_VERTEX_SHADER)
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << infoLog << std::endl;
		else if (shaderType == GL_FRAGMENT_SHADER) 
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << infoLog << std::endl;
		else
			std::cout << "ERROR::SHADER::INVALID_SHADER_TYPE " << infoLog << std::endl;
	}
}

void Shader::compileShader(GLuint& shader, const GLuint& shaderType, const std::string& shaderCode)
{
	// Simply compile specified shader
	ID = glCreateProgram();
	shader = glCreateShader(shaderType);
	const char* source = shaderCode.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	checkShaderCompile(shader, shaderType);
}

void Shader::checkShaderLink()
{
	// Simply check if shader has linked correctly
	int  success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER_LINK_FAILED " << infoLog << std::endl;
	}
}

void Shader::linkShaders(GLuint& vShader, GLuint& fShader)
{
	// Attatch specified vertex and fragment shaders
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);
	checkShaderLink();
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

void Shader::activate() const
{
	glUseProgram(ID);
}

// Following simply set the appropriate uniform variables

void Shader::setBool(const std::string& name, bool value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniform1i(loc, (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniform1i(loc, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniform1f(loc, value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1)
	{
#ifdef SHADER_UNIFORM_INFO
		std::cout << "ERROR::SHADER " << name << " INVALID" << std::endl;
#endif	
	}
	else glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader::getUniformBlockIndex(const char* name) const
{
	return glGetUniformBlockIndex(ID, name);
}
