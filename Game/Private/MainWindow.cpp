#include "MainWindow.h"
#include <ENGException.h>
#include <Helper.h>
#include <TextureLibrary.h>
#include <Logging.h>

MainWindow::MainWindow(std::string title, WindowMode mode, uvec2 size) 
	: Window(title, mode, size),
	runtime("default")
{
	
}

MainWindow::MainWindow(const char* title, WindowMode mode, uvec2 size) : MainWindow(std::string(title), mode, size)
{
	
}

void MainWindow::init()
{



	// set a background color -- it is in 0-1 scale. Pink is the best.
	glClearColor(1.f, 1.2f, .5f, 1.f);

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

	ENG_LOG("Textures Loaded");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	uint32 chunkProgram = LoadShaders("shaders\\chunkvert.glsl", "shaders\\chunkfrag.glsl");
	
	ENG_LOG("\nShaders Loaded!\n");

	aspectRatio = static_cast<float>(getSize().x) / static_cast<float>(getSize().y);
}

void MainWindow::scroll(float x, float y)
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

	// clamp scale
	scale = scale > maxScale ? maxScale : scale;
	scale = scale < minScale ? minScale : scale;

	// compute the viewMat
	viewMat = glm::ortho<float>(-aspectRatio / scale, aspectRatio / scale, -1.f / scale, 1.f / scale, .1f, 23.f);
	
	// clear the color buffer and depth buffer (makes sure the trianges in front get rendered in front
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void MainWindow::input(float deltaTime)
{
	// get the cursor position
	auto pos = getCursorLoc();
	// get the size of the window
	auto size = getSize();

	// convert to screen corrdinates (-1 to 1)
	pos /= dvec2(size);
	pos.y = 1 - pos.y;
	pos *= 2.0f;
	pos -= 1;

	// multiply by the inverse of the view matrix -- we are doing the opposite operation then usual 
	auto pos4 = vec4(pos.x, pos.y, 0.f, 1.f) * glm::inverse(viewMat);

	// convert back into vec2
	pos = vec2(pos4.x, pos4.y);



	vec2 vel(0.f);

	if (getKey(GLFW_KEY_A))
	{
		vel.x += -10.f;
	}
	if (getKey(GLFW_KEY_D))
	{
		vel.x += 10.f;
	}
	if (getKey(GLFW_KEY_W))
	{
		vel.y += 10.f;
	}
	if (getKey(GLFW_KEY_S))
	{
		vel.y += -10.f;
	}

}


void MainWindow::exit()
{

	try{
//		CLHandler::exitCL();

	} catch (ENGException& e)
	{
		ENG_LOG(e.what());
		::exit(-12);
	}
}


void MainWindow::focus(int /*focused*/)
{
	
}

void MainWindow::resize(double x, double y)
{
	aspectRatio = float(x / y);
}