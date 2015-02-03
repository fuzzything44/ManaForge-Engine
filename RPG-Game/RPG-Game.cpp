// opengl includes
#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

// opengl includes
#include <CL/cl.hpp> // .hpp has some object oriented programming approaches to it

// force use of the CL_1_2
#undef CL_VERSION_1_2

#include <CL/cl.h>
#include <CL/cl_gl.h> // interop code

// glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MainWindow.h"
#include "Helper.h"
#include "Chunk.h"
#include "CLHandler.h"

/*************** P R O T O T Y P E S ***************************************************/
GLvoid draw(GLfloat delta);
GLint init();
glm::mat4 viewMat;
float scale = 1.f;

GLuint screenX = 800, screenY = 600;

cl_int initCL();


GLint main()
{
	// bind the draw function to the draw function
	MainWindow::bindDraw(draw);

	// bind the init function to init
	MainWindow::bindInit(init);
	// run the window. consumes the thread until it returns
	return MainWindow::run(screenX, screenY, "RPG-Simulator", NULL, NULL);
	
}

GLvoid draw(GLfloat delta)
{
	// clear the color buffer and depth buffer (makes sure the trianges in front get rendered in front
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Chunk::draw();
}

GLint init()
{
	// set a background color -- it is in 0-1 scale. Pink is the best.
	glClearColor(1.f, .2f, .5f, 1.f);
	
	// enable the depth buffer so the trianges in front are in front
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	// init openCL. opencl has to be initalized after opengl and all of our buffers. 
	cl_int err = CLHandler::initCL();
	if (err != CL_SUCCESS)
	{
		return err;
	}

	// load a temp 
	GLuint width, height;
	GLuint tex = loadDDS("textures/5.dds");
	glBindTexture(GL_TEXTURE_2D, tex);

	GLuint program = LoadShaders("defaultvert.glsl", "defaultfrag.glsl");

	float aspectRatio = float((float)screenX / (float)screenY);
	
	// make the viewmat so there is no distortion based on aspect ratio.
	viewMat = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);

	Chunk::initPersistent(program, &viewMat, &scale);

	// return error code. Zero for success
	return 0;
}

