#pragma once
#include "OpenGLRendererConfig.h"

#include "WindowWidget.h"

class OpenGLRenderer;

class OpenGLWindowWidget : public WindowWidget
{
public:
	explicit OpenGLWindowWidget(OpenGLRenderer& renderer);
	virtual ~OpenGLWindowWidget() override;

	virtual const WindowProps& getWindowProps() const override;
	virtual void setWindowProps(const WindowProps& props) override;
	void saveWindowProps() override;

	virtual int32 getIsKeyPressed(const Keyboard& key) override;
	virtual vec2 getCursorLocPixels() override;

	bool shouldClose();

	virtual void draw(const mat3& mat) override;

private:
	void updateProps();

	WindowProps props;

	/// <summary> true if this object has focus.</summary>
	bool hasFocus;

	/// <summary> The window.</summary>
	GLFWwindow* window;

	static void scrollCallback(GLFWwindow* window, double x, double y);
	static void focusCallback(GLFWwindow* window, int x);

	OpenGLRenderer& renderer;
};
