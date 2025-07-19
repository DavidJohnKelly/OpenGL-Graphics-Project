#pragma once

#include <iostream>
#include <glad/glad.h>	
#include <GLFW/glfw3.h>

// Handle all OpenGL initialisation and window encapsulation
class GLWindow
{
public:
	GLWindow(int width, int height);
	operator GLFWwindow*() const { return mWindow; }

private:
	GLFWwindow* mWindow = nullptr;

	bool GLInitialise();
	void initialiseGLFeatures();
};

