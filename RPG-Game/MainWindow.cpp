#include "MainWindow.h"

using std::cout;
using std::endl;


/***************************** I N I T	 S T A T I C	  V A R I A B L E S ***********************/
GLvoid(*MainWindow::draw)(GLfloat) = NULL;
GLvoid(*MainWindow::keyboard)(GLFWwindow*, GLfloat) = NULL;
GLint(*MainWindow::init)() = NULL;
GLint(*MainWindow::exit)() = NULL;
GLvoid(*MainWindow::scroll)(GLFWwindow*, GLdouble, GLdouble) = NULL;
GLFWwindow* MainWindow::window = NULL;

GLint MainWindow::run(GLuint width, GLuint height, const GLchar* title, GLFWmonitor* monitor, GLFWwindow* share)
{
	// init GLFW (our window handler)
	int err = glfwInit();
	if (err != 1)
	{
		cout << "Failed to init GLFW" << endl;

		return err;
	}

	// set AA
	glfwWindowHint(GLFW_SAMPLES, 1);

	// set GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// set profile to core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// set the window to non-resizable
	glfwWindowHint(GLFW_RESIZABLE, false);

	// create the winodw
	window = glfwCreateWindow(width, height, title, monitor, share);
	
	// exit if the window wasn't initialized correctly
	if (!window)
	{
		fprintf(stderr, "Window failed to create");
		// terminate the glfw session
		glfwTerminate();
		return -1;
	}


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
		cout << "Failed to init GLEW. err code: " << err << endl;
		// terminate the glfw session
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// if there is a scroll function, set it to the scroll callback
	if (scroll)
		glfwSetScrollCallback(window, scroll);

	// if init exists use it and if it doesn't succede return the error code.
	if (init)
	{
		err = init();
		if (err != 0)
		{
			return err;
		}
	}

	// set initial tick
	GLfloat LastTick = glfwGetTime();

	do {
		// calculate tick time
		GLfloat CurrentTick = glfwGetTime();
		GLfloat delta = CurrentTick - LastTick;
		LastTick = CurrentTick;

		// if our keyboard function exists, then use it
		if (keyboard)
			keyboard(window, delta);

		// if our draw function exists, us it
		if (draw)
			draw(delta);

		// swap front and back buffers 
		glfwSwapBuffers(window);

		// make sure all events are done
		glfwPollEvents();
		
		// if user is pressing esc, exit the application
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	// render another frame so long the window shouldn't close. 
	//This is analogous for setting it through a function or pressing the close button
	} while (!glfwWindowShouldClose(window));

	if (exit){
		return exit();
	}
	else
	{
		return 0;
	}
}

GLvoid MainWindow::bindResize(GLvoid(*resizeIn)(GLFWwindow*, GLint, GLint))
{

	glfwSetWindowSizeCallback(window, resizeIn);
}

GLvoid MainWindow::bindDraw(GLvoid(*drawIn)(GLfloat))
{
	draw = drawIn;
}

GLvoid MainWindow::bindExit(GLint(*exitIn)())
{
	exit = exitIn;
}
GLvoid MainWindow::bindInit(GLint(*initIn)())
{
	init = initIn;
}
GLvoid MainWindow::bindKeyboard(GLvoid(*keyboardIn)(GLFWwindow*, GLfloat))
{
	keyboard = keyboardIn;
}

GLvoid MainWindow::bindScroll(GLvoid(*scrollIn)(GLFWwindow*, GLdouble, GLdouble))
{
	scroll = scrollIn;
}