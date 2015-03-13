#include "stdafx.h"

#include "MainWindow.h"


MainWindow::MainWindow(std::string title, WindowMode mode, uvec2 size) : Window(title, mode, size)
{
	
}

MainWindow::MainWindow(const char* title, WindowMode mode, uvec2 size) : Window(std::string(title), mode, size)
{

}

GLint MainWindow::init()
{

	// set a background color -- it is in 0-1 scale. Pink is the best.
	glClearColor(1.f, .2f, .5f, 1.f);

	// enable the depth buffer so the trianges in front are in front
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);


	// enable transparency
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	


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

	ENG_LOG("\nTextures Loaded!\n");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	GLuint chunkProgram = LoadShaders("shaders\\chunkvert.glsl", "shaders\\chunkfrag.glsl");
	GLuint actorProgram = LoadShaders("shaders\\actorvert.glsl", "shaders\\actorfrag.glsl");
	
	ENG_LOG("\nShaders Loaded!\n");

	// init 1 * 3 chunks of CHUNK_SIZE * CHUNK_SIZE 
	Chunk::initChunks(chunkProgram, actorProgram, &viewMat, glm::uvec2(1, 1));

	float aspectRatio = (float)getSize().x / (float)getSize().y;

	// make the projection so there is no distortion based on aspect ratio.
	projection = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);

	Actor* act = Actor::addActor<Actor>(
		vec2(-1.f, -1.f),				// Location
		vec2(1.f, 1.f),					// Size
		vec2(0.f, 0.f),					// Velocity
		0.f,							// Rotation in deg
		2,								// Render Order
		false,							// Collides	
		TextureLibrary::getUVData("0"),	// UVs
		true							// Persistent
		);							

	Actor::addActor<Actor>(
		vec2(-10.f, -10.f),				  // Location
		vec2(4.f, 4.f),					  // Size
		vec2(0.f, 0.f), 				  // Velocity
		45, 							  // Rotation in deg
		2, 								  // Render Order
		false, 							  // Collides	
		TextureLibrary::getUVData("1"),	  // UVs
		true							  // Persistent
		);

	// return error code. Zero for success
	return 0;
}

void MainWindow::scroll(GLFWwindow* window, double x, double y)
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


void MainWindow::draw(float deltaTime)
{
	// compute the viewMat
	viewMat = glm::scale(projection, glm::vec3(scale, scale, 1));

	// clear the color buffer and depth buffer (makes sure the trianges in front get rendered in front
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// temp -- will actually have player location
	Chunk::draw(vec2(0.f), deltaTime);
}


void MainWindow::input(GLFWwindow* window, float deltaTime)
{
	// get the cursor position
	dvec2 pos;
	glfwGetCursorPos(window, &pos.x, &pos.y);

	// get the size of the window
	ivec2 size;
	glfwGetWindowSize(window, &size.x, &size.y);

	// convert to screen corrdinates (-1 to 1)
	pos /= dvec2(size);
	pos.y = 1 - pos.y;
	pos *= 2.0f;
	pos -= 1;

	// multiply by the inverse of the view matrix -- we are doing the opposite operation then usual 
	vec4 pos4 = vec4(pos.x, pos.y, 0.f, 1.f) * glm::inverse(viewMat);

	// convert back into vec2
	pos = vec2(pos4.x, pos4.y);
}


GLint MainWindow::exit()
{
	CLHandler::exitCL();

	return 0;
}


GLvoid MainWindow::focus(GLFWwindow* window, int focused)
{

}

GLvoid MainWindow::resize(GLFWwindow* window, double x, double y)
{
	// compute the aspect ratio
	GLfloat aspectRatio = (GLfloat)x / (GLfloat)y;

	// update ogl to use entire screen
	glViewport(0, 0, (GLsizei)x, (GLsizei)y);

	// update the projection matrix with the new aspect ratios
	projection = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);
}