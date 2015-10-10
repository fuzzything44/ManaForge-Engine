#include "OpenGLRendererPCH.h"
#include "OpenGLWindowWidget.h"

#include "OpenGLRenderer.h"

#include <PropertyManager.h>
#include <Runtime.h>
#include <Helper.h>

OpenGLWindowWidget::OpenGLWindowWidget(OpenGLRenderer& renderer)
	: renderer(renderer)
{
	PropertyManager& propManager = Runtime::get().getPropertyManager();

	ivec2 size;
	LOAD_PROPERTY_WITH_WARNING(propManager, "window.size.x", size.x, 800);
	LOAD_PROPERTY_WITH_WARNING(propManager, "window.size.y", size.y, 600);

	LOAD_PROPERTY_WITH_WARNING(propManager, "window.renderMode", renderMode, RenderMode::NORMAL);
	LOAD_PROPERTY_WITH_WARNING(propManager, "window.windowMode", windowMode, WindowMode::FULLSCREEN);

	LOAD_PROPERTY_WITH_WARNING(propManager, "window.title", title, "WARNING- NO TITLE GIVEN");

	// init GLFW (our window handler)
	if (int err = glfwInit() != 1) {
		MFLOG(Fatal) << "Failed to init GLFW. Error code: " << err;
	}

	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mon);

	renderer.runOnRenderThreadAsync([this, mon, mode, size]
		{

			// set AA
			glfwWindowHint(GLFW_SAMPLES, 8);

			glfwWindowHint(GL_DOUBLEBUFFER, false);

			// set GL version
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

			// set profile to core profile
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// set the window to non-resizable
			glfwWindowHint(GLFW_RESIZABLE, false);

			// create the winodw
			switch (windowMode)
			{
			case WindowMode::FULLSCREEN:
				window = glfwCreateWindow(size.x, size.y, title.c_str(), mon, nullptr);
				break;
			case WindowMode::FULLSCREEN_WINDOWED:
				glfwWindowHint(GLFW_DECORATED, false);
				window = glfwCreateWindow(mode->width, mode->height, title.c_str(), nullptr, nullptr);
				break;

			case WindowMode::WINDOWED:
				window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
				break;

			default: 
				MFLOG(Error) << "Unrecgonized WindowMode.";
				break;
			}

			// exit if the window wasn't initialized correctly
			if (!window) {
				glfwTerminate();
				MFLOG(Fatal) << "\nWindow failed to create.";
			}

			glfwSetWindowUserPointer(window, this);

			// make context current in this thread
			glfwMakeContextCurrent(window);

			// make sure the cursor is shown. Most likely want to change this in the future
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			// use newer GL
			glewExperimental = GL_TRUE;

			// init GL (glew is an extension that does this for us)

			if (int err = glewInit() != GLEW_OK) {
				glfwTerminate();
				MFLOG(Fatal) << "GLEW failed to init. Error code: " << err;
			}
			// for some reason there is already an error, so clear that
			glGetError();

			// Ensure we can capture the escape key being pressed below
			glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

			glfwSetScrollCallback(window, &OpenGLWindowWidget::scrollCallback);

			glfwSetWindowFocusCallback(window, &OpenGLWindowWidget::focusCallback);
		});
}

OpenGLWindowWidget::~OpenGLWindowWidget()
{

	glfwDestroyWindow(window);

	glfwTerminate();
}

WindowWidget::WindowMode OpenGLWindowWidget::getWindowMode() 
{
	return windowMode;
}

void OpenGLWindowWidget::setRenderMode(WindowWidget::RenderMode newRenderMode) 
{
	renderMode = newRenderMode;
}
WindowWidget::RenderMode OpenGLWindowWidget::getRenderMode() const 
{
	return renderMode;
}

void OpenGLWindowWidget::setTitle(std::string& newTitle) 
{
	title = newTitle;
	glfwSetWindowTitle(window, newTitle.c_str());
}
std::string OpenGLWindowWidget::getTitle() const
{
	return title;
}

void OpenGLWindowWidget::setSize(const ivec2& newSize) 
{
	glfwSetWindowSize(window, newSize.x, newSize.y);
}
ivec2 OpenGLWindowWidget::OpenGLWindowWidget::getSize() const
{
	ivec2 ret;
	glfwGetWindowSize(window, &ret.x, &ret.y);

	return ret;
}

void OpenGLWindowWidget::setLocation(const ivec2& newLocation) 
{
	glfwSetWindowPos(window, newLocation.x, newLocation.y);
}
ivec2 OpenGLWindowWidget::getLocation() const 
{
	ivec2 ret;
	glfwGetWindowPos(window, &ret.x, &ret.y);

	return ret;
}

void OpenGLWindowWidget::setVisible(bool isVisible) 
{
	
}
bool OpenGLWindowWidget::getVisible() const 
{
	return true; // TODO: implement
}

void OpenGLWindowWidget::saveWindowProps()
{
	auto&& propManager = Runtime::get().getPropertyManager();

	ivec2 size;
	glfwGetWindowSize(window, &size.x, &size.y);
	propManager.saveValue("window.size.x", size.x);
	propManager.saveValue("window.size.y", size.y);

	propManager.saveValue("window.renderMode", static_cast<std::underlying_type_t<decltype(renderMode)>>(renderMode));
	propManager.saveValue("window.windowMode", static_cast<std::underlying_type_t<decltype(windowMode)>>(windowMode));
	propManager.saveValue("window.title", title);
}

int OpenGLWindowWidget::getIsKeyPressed(const Keyboard& key)
{
	return glfwGetKey(window, static_cast<int>(key));
}

vec2 OpenGLWindowWidget::getCursorLocPixels()
{
	dvec2 locationdouble;
	glfwGetCursorPos(window, &locationdouble.x, &locationdouble.y);

	return static_cast<vec2>(locationdouble);
}

bool OpenGLWindowWidget::shouldClose()
{
	// we need to do this because glfwWindowShouldClose returns an int as a bool -- godda love C libraries
	return glfwWindowShouldClose(window) != 0;
}

void OpenGLWindowWidget::postDraw(const mat3& /*mat*/)
{
	renderer.runOnRenderThreadAsync([this]
		{
			glfwSwapBuffers(window);
			glfwPollEvents();
		});
}

void OpenGLWindowWidget::scrollCallback(GLFWwindow* window, double x, double y)
{
	// static_cast<OpenGLWindowWidget*>(glfwGetWindowUserPointer(window))->scroll(); TODO: implement this
}

void OpenGLWindowWidget::focusCallback(GLFWwindow* window, int x)
{
	// static_cast<OpenGLWindowWidget*>(glfwGetWindowUserPointer(window))->focus(); TODO: implement this
}