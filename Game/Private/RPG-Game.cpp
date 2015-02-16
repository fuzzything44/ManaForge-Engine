// opengl includes
#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

// opencl includes
#include <CL/cl.hpp> // .hpp has some object oriented programming approaches to it

#include <CL/cl.h>
#include <CL/cl_gl.h> // interop code

// glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MainWindow.h"
#include "Helper.h"
#include "Chunk.h"
#include "CLHandler.h"
#include "TextureLibrary.h"

#include <direct.h>

/*************** P R O T O T Y P E S ***************************************************/
GLvoid draw(GLfloat delta);
GLint init();
GLvoid keyboard(GLFWwindow* window, GLfloat delta);
GLvoid scroll(GLFWwindow* winodw, GLdouble x, GLdouble y);
GLvoid resize(GLFWwindow* window, GLint x, GLint y);
GLint exit();

glm::mat4 viewMat;
glm::mat4 projection;


// 1/sclae * 2 is how many units fit vertically
GLfloat scale = .1f;

cl_int initCL();


GLint main()
{
	// changes the path so everything we open will be in Resoruce/
	char ownPth[MAX_PATH];

	// Will contain exe path
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule == NULL)
	{
		return -293;
	}
	// When passing NULL to GetModuleHandle, it returns handle of exe itself
	GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
	
	std::string path = ownPth;
	//path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 2; i++){

		
		do {
			path = path.substr(0, path.size() - 1);
		} while (path[path.size() - 1] != '\\');

	}

	_chdir(path.append("Resource\\").c_str());


	std::cout << std::endl << std::endl << std::endl;

	// bind the draw function to the draw function
	MainWindow::bindDraw(draw);
	MainWindow::bindKeyboard(keyboard);

	MainWindow::bindExit(exit);

	// bind the init function to init
	MainWindow::bindInit(init);

	MainWindow::bindScroll(scroll);
	MainWindow::bindResize(resize);

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

	// enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	CLHandler::initCL();

	TextureLibrary::addTexture("0", "textures\\0.dds");
	TextureLibrary::addTexture("1", "textures\\1.dds");
	TextureLibrary::addTexture("2", "textures\\2.dds");
	TextureLibrary::addTexture("3", "textures\\3.dds");
	TextureLibrary::addTexture("4", "textures\\4.dds");
	TextureLibrary::addTexture("5", "textures\\5.dds");
	TextureLibrary::addTexture("6", "textures\\6.dds");
	TextureLibrary::addTexture("7", "textures\\7.dds");
	TextureLibrary::addTexture("8", "textures\\8.dds");
	TextureLibrary::addTexture("9", "textures\\9.dds");
	TextureLibrary::addTexture("10", "textures\\10.dds");
	TextureLibrary::addTexture("11", "textures\\11.dds");
	TextureLibrary::addTexture("12", "textures\\12.dds");
	TextureLibrary::addTexture("13", "textures\\13.dds");
	TextureLibrary::addTexture("14", "textures\\14.dds");
	TextureLibrary::addTexture("15", "textures\\15.dds");
	TextureLibrary::addTexture("16", "textures\\16.dds");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLuint program = LoadShaders("shaders\\chunkvert.glsl", "shaders\\chunkfrag.glsl");

	Chunk::initChunks(program, &viewMat, glm::uvec2(1, 3));
	

	float aspectRatio = (float)MainWindow::getWindowWidth() / (float)MainWindow::getWindowHeight();

	// make the projection so there is no distortion based on aspect ratio.
	projection = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);


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

GLint exit()
{
	CLHandler::exitCL();

	return 0;
}
