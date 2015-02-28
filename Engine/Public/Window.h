#pragma once
#include <Engine.h>
#include <map>


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
	ENGINE_API Window(std::string title, WindowMode mode, uvec2 size);

	// Runs the window
	ENGINE_API virtual GLint run();

	uvec2 ENGINE_API getSize();

protected:

	std::string title;
	WindowMode mode;
	 
	ENGINE_API void updateWindowParameters();
	 
	bool hasFocus;
	 
	GLFWwindow* window;
	 
	ENGINE_API virtual GLint init() = 0;
	ENGINE_API virtual void scroll(GLFWwindow* window, double x, double y) = 0;
	ENGINE_API virtual void draw(float deltaTime) = 0;
	ENGINE_API virtual void input(GLFWwindow* window, float deltaTime) = 0;
	ENGINE_API virtual GLint exit() = 0;
	ENGINE_API virtual GLvoid focus(GLFWwindow* window, int focused) = 0;
	 
	ENGINE_API static std::map<GLFWwindow*, Window*> windows;
	 
	ENGINE_API static void scrollCallback(GLFWwindow* window, double x, double y)
	{
		if (Window* winObj = windows[window])
		{
			winObj->scroll(window, x, y);
		}
	}

	ENGINE_API static void focusCallback(GLFWwindow* window, int focused)
	{
		if (Window* winObj = windows[window])
		{
			winObj->focus(window, focused);
		}
	}
};

