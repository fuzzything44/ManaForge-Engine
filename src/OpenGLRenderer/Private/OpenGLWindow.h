#pragma once
#include "OpenGLRendererConfig.h"

#include "Window.h"

class OpenGLRenderer;

class OpenGLWindow : public Window
{
public:
	explicit OpenGLWindow(OpenGLRenderer& renderer);
	virtual ~OpenGLWindow() override;

	virtual const WindowProps& getWindowProps() const override;
	virtual void setWindowProps(const WindowProps& props) override;
	void saveWindowProps() override;

	virtual int32 getIsKeyPressed(Keyboard key) override;
	virtual vec2 getCursorLocPixels() override;

	virtual void swapBuffers();
	virtual void pollEvents();
	bool shouldClose();

private:
	void updateProps();

	WindowProps props;

	/// <summary> true if this object has focus.</summary>
	bool hasFocus;

	/// <summary> The window.</summary>
	GLFWwindow* window;

	static std::map<GLFWwindow*, OpenGLWindow*> windows;

	static void scrollCallback(GLFWwindow* window, double x, double y);
	static void focusCallback(GLFWwindow* window, int x);

	OpenGLRenderer& renderer;
};
