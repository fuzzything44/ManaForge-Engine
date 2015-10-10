#pragma once
#include "OpenGLRendererConfig.h"

#include "WindowWidget.h"

class OpenGLRenderer;

class OpenGLWindowWidget : public WindowWidget
{
public:
	explicit OpenGLWindowWidget(OpenGLRenderer& renderer);
	virtual ~OpenGLWindowWidget() override;

	virtual WindowMode getWindowMode() override;

	virtual void setRenderMode(RenderMode newRenderMode) override;
	virtual RenderMode getRenderMode() const override;

	virtual void setTitle(std::string& newTitle) override;
	virtual std::string getTitle() const override;

	virtual void setSize(const ivec2& newSize) override;
	virtual ivec2 getSize() const override;

	virtual void setLocation(const ivec2& newLocation) override;
	virtual ivec2 getLocation() const override;

	virtual void setVisible(bool isVisible) override;
	virtual bool getVisible() const override;

	void saveWindowProps() override;

	virtual int32 getIsKeyPressed(const Keyboard& key) override;
	virtual vec2 getCursorLocPixels() override;

	bool shouldClose();

	virtual void postDraw(const mat3& mat) override;

private:

	/// <summary> The window.</summary>
	GLFWwindow* window;

	static void scrollCallback(GLFWwindow* window, double x, double y);
	static void focusCallback(GLFWwindow* window, int x);

	OpenGLRenderer& renderer;

	RenderMode renderMode;
	WindowMode windowMode;
	std::string title;
};
