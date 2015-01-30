// opengl includes
#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

// opengl includes
#include <CL/cl.hpp> // .hpp has some object oriented programming approaches to it

// force use of the CL_1_2
#undef CL_VERSION_1_2

#include <CL/cl.h>
#include <CL/cl_gl.h> // interop code

#include "MainWindow.h"
#include "Helper.h"
#include "CLHandler.h"

/*************** P R O T O T Y P E S ***************************************************/
GLvoid draw(GLfloat delta);
GLint init();

cl_int initCL();


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
	



	// init openCL. opencl has to be initalized after opengl and all of our buffers. 
	cl_int err = CLHandler::initCL();

	

	// return error code. Zero for success
	return 0;
}

