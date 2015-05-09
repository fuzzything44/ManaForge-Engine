#pragma once

#include "Window.h"

class OpenGLWindow : public Window
{
public:

	explicit OpenGLWindow(const WindowProps& props = WindowProps());

	virtual void setRenderMode(RenderMode newMode) override;
	virtual RenderMode getRenderMode() override;
	virtual void init() override;
	virtual uvec2 getSize() const override;
	virtual int32 getKey(Keyboard key) override;
	virtual vec2 getCursorLocPixels() override;
	 
	virtual void swapBuffers();
	virtual void pollEvents();

private:
	WindowProps props;

	/// <summary> true if this object has focus.</summary>
	bool hasFocus;

	/// <summary> The window.</summary>
	GLFWwindow* window;

	std::map<GLFWwindow*, OpenGLWindow*> windows;

	static void scrollCallback(GLFWwindow* window, double x, double y);
	static void focusCallback(GLFWwindow* window, int x);
};
