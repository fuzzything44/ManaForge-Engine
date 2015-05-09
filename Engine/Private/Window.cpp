#include "Window.h"
#include <sstream>
#include "Logging.h"


void Window::run()
{
	
	
}


int32 Window::getKey(Keyboard key)
{
	return glfwGetKey(window, key);
}

vec2 Window::getCursorLoc()
{
	double x, y;

	glfwGetCursorPos(window, &x, &y);

	return vec2(static_cast<float>(x), static_cast<float>(y));
}

void Window::setRenderMode(RenderMode newMode)
{
	props.renderMode = newMode;
}

RenderMode Window::getRenderMode()
{
	return props.renderMode;
}

uvec2 Window::getSize() const
{
	ivec2 size;

	glfwGetWindowSize(window, &size.x, &size.y);

	return uvec2(size);
}

void Window::init()
{

}

void Window::scroll(float x, float y)
{

}

void Window::focus(int in)
{
	hasFocus = in == 0 ? false : true;
}

void Window::scrollCallback(GLFWwindow* window, double x, double y)
{
		
	if (Window* winObj = windows[window])
	{
		winObj->scroll((float)x, (float)y);
	}
}

void Window::focusCallback(GLFWwindow* window, int focused)
{
		
	if (Window* winObj = windows[window])
	{
		winObj->focus(focused);
	}
}