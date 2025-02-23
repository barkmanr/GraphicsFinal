#include "WindowController.h"

WindowController::WindowController()
{
	window = nullptr;
	resolution = Resolution(1280,768 );
}

WindowController::~WindowController()
{
	if (window != nullptr)
	{
		glfwTerminate();
		window = nullptr;
	}
}

void WindowController::Create(int _width, int _height)
{
	if (_width == -1 || _height == -1)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		resolution = Resolution(mode->width, mode->height);
	}
	else 
	{

		resolution = Resolution(_width, _height);
	}
	
}
void WindowController::NewWindow()
{
	M_ASSERT(glfwInit(), "Failed to initialize GLFW.");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	Resolution r = WindowController::GetResolution();
	M_ASSERT((window = glfwCreateWindow(r.width,r.height, "A sample scene", NULL, NULL)) != nullptr, "Failed to open GLFW window"); //size and title of window
	glfwMakeContextCurrent(window);
}

