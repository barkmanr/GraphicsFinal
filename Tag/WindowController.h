#ifndef WINDOW_CONTROLLER_H
#define WINDOW_CONTROLLER_H

#include "StandardIncludes.h"

class WindowController : public Singleton<WindowController>
{
public:
	WindowController();
	virtual ~WindowController();

	void Create(int _width = -1, int height = -1);

	GLFWwindow* GetWindow() //Returns the window
	{
		if (window == nullptr)
		{
			NewWindow();
		}
		return window;
	}

	void NewWindow();
	const Resolution& GetResolution() { return resolution; };

private:
	GLFWwindow* window;
	Resolution resolution;
};

#endif WINDOW_CONTROLLER_H
