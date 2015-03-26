#pragma once
#include <Engine.h>
#include <map>

/// <summary> Values that represent window modes.</summary>
enum WindowMode
{
	FULLSCREEN,
	FULLSCREEN_WINDOWED,
	WINDOWED
};

/// <summary> Values that represent render modes.</summary>
enum RenderMode
{
	NORMAL,
	WIREFRAME
};

/// <summary> A window.</summary>
class Window
{
public:


	ENGINE_API void setRenderMode(RenderMode newMode);

	ENGINE_API Window(std::string title, WindowMode mode, uvec2 size);

	ENGINE_API virtual GLint run();

	uvec2 ENGINE_API getSize();

	ENGINE_API GLint getKey(int key);
	ENGINE_API vec2 getCursorLoc();

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

	RenderMode renderMode;




	ENGINE_API static void focusCallback(GLFWwindow* window, int focused)
	{
		if (Window* winObj = windows[window])
		{
			winObj->focus(window, focused);
		}
	}
};

