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

	if (!glfwInit())
	{
		cout << "Failed to init GLFW" << endl;

		return -1;
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

	window = glfwCreateWindow(width, height, title, monitor, share);
	

	// exit if the window wasn't initialized correctly
	if (!window)
	{
		fprintf(stderr, "Window failed to create");
		glfwTerminate();
		return -1;
	}


	// make context current
	glfwMakeContextCurrent(window);

	// use newer GL
	glewExperimental = GL_TRUE;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	



	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to init GLEW. err code: "<< glewInit() << endl;
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	if (scroll)
		glfwSetScrollCallback(window, scroll);

	if (init)
	{
		GLint errCode = init();
		if (errCode != 0)
		{
			return errCode;
		}
	}

	GLfloat LastTick = glfwGetTime();

	do {
		GLfloat CurrentTick = glfwGetTime();
		GLfloat delta = CurrentTick - LastTick;
		LastTick = CurrentTick;

		if (keyboard)
			keyboard(window, delta);

		if (draw)
			draw(delta);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

	} while (!glfwWindowShouldClose(window));


	return exit();
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