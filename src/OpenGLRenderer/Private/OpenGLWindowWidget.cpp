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

	LOAD_PROPERTY_WITH_WARNING(propManager, "window.size.x", props.size.x, 800);
	LOAD_PROPERTY_WITH_WARNING(propManager, "window.size.y", props.size.y, 600);

	LOAD_PROPERTY_WITH_WARNING(propManager, "window.renderMode", props.renderMode, RenderMode::NORMAL);
	LOAD_PROPERTY_WITH_WARNING(propManager, "window.windowMode", props.windowMode, WindowMode::FULLSCREEN);

	LOAD_PROPERTY_WITH_WARNING(propManager, "window.title", props.title, "WARNING- NO TITLE GIVEN");

	hasFocus = true;

	// init GLFW (our window handler)
	if (int err = glfwInit() != 1) {
		MFLOG(Fatal) << "Failed to init GLFW. Error code: " << err;
	}

	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mon);

	renderer.runOnRenderThreadAsync([this, mon, mode]
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
			switch (props.windowMode)
			{
			case WindowMode::FULLSCREEN:
				window = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), mon, nullptr);
				break;
			case WindowMode::FULLSCREEN_WINDOWED:
				glfwWindowHint(GLFW_DECORATED, false);
				window = glfwCreateWindow(mode->width, mode->height, props.title.c_str(), nullptr, nullptr);

				props.size = uvec2(mode->width, mode->height);
				break;

			case WindowMode::WINDOWED:
				window = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), nullptr, nullptr);
				break;

			default: break;
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

const WindowProps& OpenGLWindowWidget::getWindowProps() const { return props; }

void OpenGLWindowWidget::setWindowProps(const WindowProps& props)
{
	this->props = props;

	updateProps();
}

void OpenGLWindowWidget::saveWindowProps()
{
	auto&& propManager = Runtime::get().getPropertyManager();

	propManager.saveValue("window.size.x", props.size.x);
	propManager.saveValue("window.size.y", props.size.y);

	propManager.saveValue("window.renderMode", static_cast<uint8>(props.renderMode));
	propManager.saveValue("window.windowMode", static_cast<uint8>(props.windowMode));
	propManager.saveValue("window.title", props.title);
}

int OpenGLWindowWidget::getIsKeyPressed(const Keyboard& key) { return glfwGetKey(window, static_cast<int>(key)); }

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

void OpenGLWindowWidget::draw(const mat3 & mat)
{
	renderer.runOnRenderThreadAsync([this]
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	});
}

void OpenGLWindowWidget::updateProps()
{
	glfwSetWindowSize(window, props.size.x, props.size.y);
	glfwSetWindowTitle(window, props.title.c_str());

	glViewport(0, 0, props.size.x, props.size.y);

	switch (props.renderMode)
	{
	case RenderMode::NORMAL: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	case RenderMode::WIREFRAME: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	default: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	}

	// show the window if it should be visible
	props.visible ? glfwShowWindow(window) : glfwHideWindow(window);
}

void OpenGLWindowWidget::scrollCallback(GLFWwindow* window, double x, double y)
{
	// static_cast<OpenGLWindowWidget*>(glfwGetWindowUserPointer(window))->scroll(); TODO: implement this
}

void OpenGLWindowWidget::focusCallback(GLFWwindow* window, int x)
{
	// static_cast<OpenGLWindowWidget*>(glfwGetWindowUserPointer(window))->focus(); TODO: implement this

}