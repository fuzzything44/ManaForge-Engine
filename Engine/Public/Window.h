#pragma once
#include <Engine.h>
#include <map>

/**
 * \enum	WindowMode
 *
 * \brief	enum for the window modes.
 */

enum WindowMode
{
	FULLSCREEN,
	FULLSCREEN_WINDOWED,
	WINDOWED
};

enum RenderMode
{
	NORMAL,
	WIREFRAME
};

/**
 * \class	Window
 *
 * \brief	Handler class for window.
 *
 * \author	Russell
 * \date	3/12/2015
 */

class Window
{
public:

	/**
	 * \fn	ENGINE_API Window::Window(std::string title, WindowMode mode, uvec2 size);
	 *
	 * \brief	Creates the window.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	title	The title of the winodw.
	 * \param	mode 	The mode to run the window in.
	 * \param	size 	The size of the window.
	 *
	 * \return	An ENGINE_API.
	 */

	ENGINE_API void setRenderMode(RenderMode newMode);

	ENGINE_API Window(std::string title, WindowMode mode, uvec2 size);

	/**
	 * \fn	ENGINE_API virtual GLint Window::run();
	 *
	 * \brief	Gets the run.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	A GLint.
	 */

	ENGINE_API virtual GLint run();

	/**
	 * \fn	uvec2 ENGINE_API Window::getSize();
	 *
	 * \brief	Gets the size.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The size.
	 */

	uvec2 ENGINE_API getSize();

	ENGINE_API GLint getKey(int key);
	ENGINE_API vec2 getCursorLoc();

protected:

	std::string title;
	WindowMode mode;

	/**
	 * \fn	ENGINE_API void Window::updateWindowParameters();
	 *
	 * \brief	Updates the window parameters.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 */

	ENGINE_API void updateWindowParameters();
	 
	bool hasFocus;
	 
	GLFWwindow* window;

	/**
	 * \fn	ENGINE_API virtual GLint Window::init() = 0;
	 *
	 * \brief	Initialises this object.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	A GLint.
	 */

	ENGINE_API virtual GLint init() = 0;

	/**
	 * \fn	ENGINE_API virtual void Window::scroll(GLFWwindow* window, double x, double y) = 0;
	 *
	 * \brief	Scrolls.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param [in,out]	window	If non-null, the window.
	 * \param	x			  	The x coordinate.
	 * \param	y			  	The y coordinate.
	 */

	ENGINE_API virtual void scroll(GLFWwindow* window, double x, double y) = 0;

	/**
	 * \fn	ENGINE_API virtual void Window::draw(float deltaTime) = 0;
	 *
	 * \brief	Draws.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	deltaTime	The delta time.
	 */

	ENGINE_API virtual void draw(float deltaTime) = 0;

	/**
	 * \fn	ENGINE_API virtual void Window::input(GLFWwindow* window, float deltaTime) = 0;
	 *
	 * \brief	Inputs.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param [in,out]	window	If non-null, the window.
	 * \param	deltaTime	  	The delta time.
	 */

	ENGINE_API virtual void input(GLFWwindow* window, float deltaTime) = 0;

	/**
	 * \fn	ENGINE_API virtual GLint Window::exit() = 0;
	 *
	 * \brief	Gets the exit.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	A GLint.
	 */

	ENGINE_API virtual GLint exit() = 0;

	/**
	 * \fn	ENGINE_API virtual GLvoid Window::focus(GLFWwindow* window, int focused) = 0;
	 *
	 * \brief	Focus.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param [in,out]	window	If non-null, the window.
	 * \param	focused		  	The focused.
	 *
	 * \return	A GLvoid.
	 */

	ENGINE_API virtual GLvoid focus(GLFWwindow* window, int focused) = 0;
	 
	ENGINE_API static std::map<GLFWwindow*, Window*> windows;

	/**
	 * \fn	ENGINE_API static void Window::scrollCallback(GLFWwindow* window, double x, double y)
	 *
	 * \brief	Callback, called when the scroll.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param [in,out]	window	If non-null, the window.
	 * \param	x			  	The x coordinate.
	 * \param	y			  	The y coordinate.
	 */

	ENGINE_API static void scrollCallback(GLFWwindow* window, double x, double y)
	{
		if (Window* winObj = windows[window])
		{
			winObj->scroll(window, x, y);
		}
	}

	RenderMode renderMode;


	/**
	 * \fn	ENGINE_API static void Window::focusCallback(GLFWwindow* window, int focused)
	 *
	 * \brief	Callback, called when the focus.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param [in,out]	window	If non-null, the window.
	 * \param	focused		  	The focused.
	 */

	ENGINE_API static void focusCallback(GLFWwindow* window, int focused)
	{
		if (Window* winObj = windows[window])
		{
			winObj->focus(window, focused);
		}
	}
};

