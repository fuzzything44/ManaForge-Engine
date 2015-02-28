#pragma once
#include <Engine.h>
#include <Logging.h>
#include <Helper.h>
#include <iostream>
#include <map>

#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

#include <glm/glm.hpp>

// enum for the window modes
enum WindowMode
{
	FULLSCREEN,
	FULLSCREEN_WINDOWED,
	WINDOWED
};

// Handler class for window
class Window
{
public:

	/// <summary> Creates the window </summary>
	/// <param name='title'> The title of the winodw </param>
	/// <param name='mode'> The mode to run the window in </param>
	/// <param name='size'> The size of the window
	Window(std::string title, WindowMode mode, uvec2 size);

	// Runs the window
	virtual GLint run();

	uvec2 getSize();

protected:

	std::string title;
	WindowMode mode;

	void updateWindowParameters();

	bool hasFocus;

	GLFWwindow* window;
	
	virtual GLint init() = 0;
	virtual void scroll(GLFWwindow* window, double x, double y) = 0;
	virtual void draw(float deltaTime) = 0;
	virtual void input(GLFWwindow* window, float deltaTime) = 0;
	virtual GLint exit() = 0;
	virtual GLvoid focus(GLFWwindow* window, int focused) = 0;

	static std::map<GLFWwindow*, Window*> windows;

	static void scrollCallback(GLFWwindow* window, double x, double y)
	{
		if (Window* winObj = windows[window])
		{
			winObj->scroll(window, x, y);
		}
	}

	static void focusCallback(GLFWwindow* window, int focused)
	{
		if (Window* winObj = windows[window])
		{
			winObj->focus(window, focused);
		}
	}
};

