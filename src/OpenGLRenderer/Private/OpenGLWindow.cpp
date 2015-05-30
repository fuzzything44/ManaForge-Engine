#include "OpenGLRendererConfig.h"

#include "OpenGLWindow.h"
#include "Logging.h"
#include <Runtime.h>

std::map<GLFWwindow*, OpenGLWindow*> OpenGLWindow::windows = std::map<GLFWwindow*, OpenGLWindow*>();

OpenGLWindow::OpenGLWindow() 
	: Window()
{
	PropertyManager& propManager = Runtime::get().propManager;

	props.size.x = propManager.queryValue<uint32>("window.size.x");
	props.size.y = propManager.queryValue<uint32>("window.size.y");

	props.renderMode = static_cast<RenderMode>(propManager.queryValue<uint8>("window.renderMode"));
	props.windowMode = static_cast<WindowMode>(propManager.queryValue<uint8>("window.windowMode"));

	props.title = propManager.queryValue<std::string>("window.title");

	

	hasFocus = true;

	// init GLFW (our window handler)
	if (int err = glfwInit() != 1)
	{
		FATAL_ERR("Failed to init GLFW. Error code: " + err);
	}

	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mon);

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
	switch (props.windowMode)
	{
	case WindowMode::FULLSCREEN:
		window = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), mon, nullptr);
		break;
	case WindowMode::FULLSCREEN_WINDOWED:
		glfwWindowHint(GLFW_DECORATED, false);
		window = glfwCreateWindow(mode->width, mode->height, props.title.c_str(), nullptr, nullptr);
		break;

	case WindowMode::WINDOWED:
		window = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), nullptr, nullptr);
		break;

	default:

		break;
	}



	// exit if the window wasn't initialized correctly
	if (!window)
	{
		glfwTerminate();
		FATAL_ERR("\nWindow failed to create.");
	}


	windows[window] = this;

	// make context current in this thread
	glfwMakeContextCurrent(window);


	// make sure the cursor is shown. Most likely want to change this in the future
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	// use newer GL
	glewExperimental = GL_TRUE;

	// init GL (glew is an extension that does this for us)
	
	if (int err = glewInit() != GLEW_OK)
	{
		glfwTerminate();
		FATAL_ERR("GLEW failed to init. Error code: " + err);
	}
	// for some reason there is already an error, so clear that
	glGetError();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	glfwSetScrollCallback(window, &OpenGLWindow::scrollCallback);


	glfwSetWindowFocusCallback(window, &OpenGLWindow::focusCallback);
}

OpenGLWindow::~OpenGLWindow()
{
}

const WindowProps& OpenGLWindow::getWindowProps() const
{
	return props;
}

void OpenGLWindow::setWindowProps(const WindowProps& props)
{
	this->props = props;

	updateProps();
}

void OpenGLWindow::saveWindowProps()
{
	Runtime::get().propManager.saveValue("window.size.x", props.size.x);
	Runtime::get().propManager.saveValue("window.size.y", props.size.y);

	Runtime::get().propManager.saveValue("window.renderMode", static_cast<uint8>(props.renderMode));
	Runtime::get().propManager.saveValue("window.windowMode", static_cast<uint8>(props.windowMode));
	Runtime::get().propManager.saveValue("window.title", props.title);
}

int OpenGLWindow::getIsKeyPressed(Keyboard key)
{
	return glfwGetKey(window, static_cast<int>(key));
}

vec2 OpenGLWindow::getCursorLocPixels()
{
	dvec2 locationdouble;
	glfwGetCursorPos(window, &locationdouble.x, &locationdouble.y);

	return static_cast<vec2>(locationdouble);
}

void OpenGLWindow::swapBuffers()
{
	glfwSwapBuffers(window);
}

void OpenGLWindow::pollEvents()
{
	glfwPollEvents();
}

bool OpenGLWindow::shouldClose()
{
	// we need to do this because glfwWindowShouldClose returns an int as a bool -- godda love C libraries
	return glfwWindowShouldClose(window) != 0;
}

void OpenGLWindow::updateProps()
{
	glfwSetWindowSize(window, props.size.x, props.size.y);
	glfwSetWindowTitle(window, props.title.c_str());

	switch (props.renderMode)
	{
	case RenderMode::NORMAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case RenderMode::WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	default:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	// show the window if it should be visible
	props.visible ? glfwShowWindow(window) : glfwHideWindow(window);
	

}

void OpenGLWindow::scrollCallback(GLFWwindow* window, double x, double y)
{
	auto iter = windows.find(window);
	if (iter != windows.end())
	{
		// iter->second->scroll(x, y);
	}
}

void OpenGLWindow::focusCallback(GLFWwindow* window, int x)
{
	auto iter = windows.find(window);
	if (iter != windows.end())
	{
		// iter->second->focus(x);
	}
}