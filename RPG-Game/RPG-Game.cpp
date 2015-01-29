// opengl includes
#include <GLTools/glew.h>
#include <GLTools/glfw3.h>


#include "MainWindow.h"
#include "Helper.h"

GLvoid draw(GLfloat delta);
GLint init();

GLint main()
{
	// bind the draw function to the draw function
	MainWindow::bindDraw(draw);

	// bind the init function to init
	MainWindow::bindInit(init);

	// run the window. consumes the thread until it returns
	return MainWindow::run(800, 600, "RPG-Simulator", NULL, NULL);
}

GLvoid draw(GLfloat delta)
{
	// clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);
}

GLint init()
{
	// set a background color -- it is in 0-1 scale. Pink is the best.
	glClearColor(1.f, .2f, .5f, 1.f);
	


	// return error code. Zero for success
	return 0;
}