#include "GLWindow.h"

GLWindow::GLWindow(int width, int height)
{
	// Initialise and error check all OpenGL steps

	if (!GLInitialise())
	{
		std::cerr << "***Failed to initialise OpenGL***\n" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	mWindow = glfwCreateWindow(width, height, "OpenGL Project", NULL, NULL);
	if (!mWindow)
	{
		std::cerr << "***Failed to initialise window***\n" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(mWindow);

	// Load opengl functions into memory
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "***Failed to initialize GLAD***\n" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	glViewport(0, 0, width, height);
	initialiseGLFeatures();
}

bool GLWindow::GLInitialise() {
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16); // Hint to use MSAA anti aliasing

	return true;
}

void GLWindow::initialiseGLFeatures()
{
	glEnable(GL_CULL_FACE); // Counter clockwise with back face culling

	// Enable depth testing
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL); // for skybox
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Stop cubemap seams in corners and edges

	glEnable(GL_MULTISAMPLE); // Enable MSAA anti aliasing

	// Allow for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
