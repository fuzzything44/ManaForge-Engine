#include "Window.h"
#include <sstream>
#include "Logging.h"
#include <ENGException.h>

std::map<GLFWwindow*, Window*> Window::windows = std::map<GLFWwindow*, Window*>();

Window::Window(std::string title, WindowMode windowmode, uvec2 size)
{STACK
	hasFocus = true;

	// init GLFW (our window handler)
	int err = glfwInit();
	if (err != 1)
	{
		ENG_LOG("Failed to init GLFW" << std::endl);

		return;
	}
	
	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mon);
	
	// set AA
	glfwWindowHint(GLFW_SAMPLES, 8);

	glfwWindowHint(GL_DOUBLEBUFFER, false);
	
	

	// set GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// we don't want a border if it is fullscreen windowed, otherwise we do
	if (windowmode == FULLSCREEN_WINDOWED){

		glfwWindowHint(GLFW_DECORATED, false);
	}
	else
	{
		glfwWindowHint(GLFW_DECORATED, true);
	}

	// set profile to core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// set the window to non-resizable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create the winodw
	switch (windowmode)
	{
	case FULLSCREEN:
		window = glfwCreateWindow(size.x, size.y, title.c_str(), mon, nullptr);
		break;
	case FULLSCREEN_WINDOWED:
		window = glfwCreateWindow(mode->width, mode->height, title.c_str(), nullptr, nullptr);
		break;

	case WINDOWED:
		window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		break;

	default:

		break;
	}


	
	// exit if the window wasn't initialized correctly
	if (!window)
	{
		ENG_LOG("\nWindow failed to create. Exiting");
		// terminate the glfw session
		glfwTerminate();
		return;
	}


	windows[window] = this;

	// make context current
	glfwMakeContextCurrent(window);

	// use newer GL
	glewExperimental = GL_TRUE;

	// make sure the cursor is shown. Most likely want to change this in the future
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	


	// init GL (glew is an extension that does this for us)
	err = glewInit();
	if (err != GLEW_OK)
	{
		ENG_LOG("Failed to init GLEW. err code: ");
		// terminate the glfw session
		glfwTerminate();
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	
	glfwSetScrollCallback(window, &Window::scrollCallback);
	

	glfwSetWindowFocusCallback(window, &Window::focusCallback);

}

void Window::run()
{STACK
	// if init exists use it and if it doesn't succede return the error code.

	double start = glfwGetTime();

	try
	{
		init();
	}
	catch (std::exception& e)
	{
		ENG_LOG(e.what());
		
	}

	ENG_LOG("Init finished in " << glfwGetTime() - start << "s");


	// set initial tick
	float LastTick = static_cast<float>(glfwGetTime());

	try
	{
		do {

			if (hasFocus){

				// calculate tick time
				float CurrentTick = static_cast<float>(glfwGetTime());
				float delta = CurrentTick - LastTick;

				LastTick = CurrentTick;


				input(delta);


				draw(delta);

				// swap front and back buffers 
				glfwSwapBuffers(window);


				// if user is pressing esc, exit the application
				if (glfwGetKey(window, GLFW_KEY_ESCAPE))
				{
					glfwSetWindowShouldClose(window, GL_TRUE);
				}
				// render another frame so long the window shouldn't close. 
				// This is analogous for setting it through a function or pressing the close button

			}

			// make sure all events are done
			glfwPollEvents();

		} while (!glfwWindowShouldClose(window));
	}
	catch (ENGException& e)
	{
		ENG_LOG(e.what());

		::exit(-2);
	}
	exit();
	
}


int32 Window::getKey(int key)
{STACK
	return glfwGetKey(window, key);
}

vec2 Window::getCursorLoc()
{STACK
	double x, y;

	glfwGetCursorPos(window, &x, &y);

	return vec2(static_cast<float>(x), static_cast<float>(y));
}

void Window::setRenderMode(RenderMode newMode)
{STACK
	renderMode = newMode;
}

RenderMode Window::getRenderMode()
{STACK
	return renderMode;
}

uvec2 Window::getSize() const
{STACK
	ivec2 size;

	glfwGetWindowSize(window, &size.x, &size.y);

	return uvec2(size);
}

void Window::focus(int in)
{STACK
	hasFocus = in == 0 ? false : true;
}

