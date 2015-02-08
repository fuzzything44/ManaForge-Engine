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
GLvoid keyboard(GLFWwindow* window, GLfloat delta);
GLvoid scroll(GLFWwindow* winodw, GLdouble x, GLdouble y);
GLvoid resize(GLFWwindow* window, GLint x, GLint y);

glm::mat4 viewMat;
glm::mat4 projection;


// 1/sclae * 2 is how many units fit vertically
GLfloat scale = 1;

cl_int initCL();


GLint main()
{


	std::cout << std::endl << std::endl << std::endl;

	// bind the draw function to the draw function
	MainWindow::bindDraw(draw);
	MainWindow::bindKeyboard(keyboard);

	MainWindow::bindScroll(scroll);

	// bind the init function to init
	MainWindow::bindInit(init);


	// run the window. consumes the thread until it returns
	return MainWindow::run("RPG-Simulator", GL_FALSE);
	
}


GLvoid keyboard(GLFWwindow* window, GLfloat delta)
{
	
}

GLvoid draw(GLfloat delta)
{
	// compute the viewMat
	viewMat = glm::scale(projection, glm::vec3(scale, scale, 1.f));

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, tex);

	GLuint program = LoadShaders("chunkvert.glsl", "chunkfrag.glsl");

	GLint widthWin, heightWin;

	glfwGetWindowSize(MainWindow::window, &widthWin, &heightWin);

	float aspectRatio = (float)widthWin / (float)heightWin;
	
	// make the projection so there is no distortion based on aspect ratio.
	projection = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);

	Chunk::initPersistent(program, &viewMat);

	Chunk::addChunk(program, &viewMat, glm::vec2(0.f, 0.f));
	Chunk::addChunk(program, &viewMat, glm::vec2(CHUNK_WIDTH, 0.f));
	Chunk::addChunk(program, &viewMat, glm::vec2(0.f, CHUNK_WIDTH));
	Chunk::addChunk(program, &viewMat, glm::vec2(CHUNK_WIDTH, CHUNK_WIDTH));

	glfwSetScrollCallback(MainWindow::window, scroll);
	glfwSetWindowSizeCallback(MainWindow::window, resize);

	// return error code. Zero for success
	return 0;
}

GLvoid resize(GLFWwindow* window, GLint x, GLint y)
{
	// compute the aspect ratio
	GLfloat aspectRatio = (GLfloat)x / (GLfloat)y;

	// update ogl to use entire screen
	glViewport(0, 0, x, y);

	// update the projection matrix with the new aspect ratios
	projection = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);
}

GLvoid scroll(GLFWwindow* winodw, GLdouble x, GLdouble y)
{
	if (x != 0 || y != 0)
	{
		if (y == 1.f)
		{ 
			// scale the scale
			scale *= 2.f;
		}
		else if (y == -1.f)
		{
			// scale the scale variable
			scale /= 2.f;
		}

	}
}