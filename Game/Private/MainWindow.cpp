#include "MainWindow.h"
#include <ModuleManager.h>
#include <ENGException.h>
#include <Helper.h>
#include <TextureLibrary.h>
#include <random>

MainWindow::MainWindow(std::string title, WindowMode mode, uvec2 size) : Window(title, mode, size)
{STACK
	
}

MainWindow::MainWindow(const char* title, WindowMode mode, uvec2 size) : Window(std::string(title), mode, size)
{STACK
	
}

void MainWindow::init()
{STACK

	man.loadModule("OpenGLRenderer");
	man.loadModule("DefaultWorld");	

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

	ENG_LOG("\nTextures Loaded!\n");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	uint32 chunkProgram = LoadShaders("shaders\\chunkvert.glsl", "shaders\\chunkfrag.glsl");
	uint32 actorProgram = LoadShaders("shaders\\actorvert.glsl", "shaders\\actorfrag.glsl");
	
	ENG_LOG("\nShaders Loaded!\n");

	// init 1 * 3 chunks of CHUNK_SIZE * CHUNK_SIZE 
	Chunk::initChunks(chunkProgram, &viewMat, glm::uvec2(1, 1));
	Actor::init(actorProgram, &viewMat);

	aspectRatio = static_cast<float>(getSize().x) / static_cast<float>(getSize().y);


	srand(reinterpret_cast<uint32>(this));
	int seed = rand();
	ENG_LOG(std::endl << "using seed: " << seed << std::endl)

	// init the generator -- will remove with loading
	std::mersenne_twister_engine < std::uint_fast32_t, 32, 624, 397, 31,
		0x9908b0df, 11,
		0xffffffff, 7,
		0x9d2c5680, 15,
		0xefc60000, 18, 1812433253 > gen(rand());

	for (int i = 0; i < 10; i++)
	{


		srand(i);
		Actor::addActor<Actor>(
			vec2(std::uniform_real_distribution<float>(-10.f, 0.f)(gen), std::uniform_real_distribution<float>(-10.f, 0.f)(gen)),
			vec2(.5f, .5f),
			vec2(0.f, 0.f),
			float(std::uniform_real_distribution<float>(0.f, 360.f)(gen)),
			uint8(3),
			true,
			TextureLibrary::getUVData("9"),
			true
			);
	}

	character = Actor::addActor<Actor>(
		vec2(0.f, 0.f),
		vec2(2.f, 2.f),
		vec2(0.f, 0.f),
		200.f,
		0,
		true,
		TextureLibrary::getUVData("1"),
		true
		);

}

void MainWindow::scroll(float x, float y)
{STACK
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
{STACK

	// clamp scale
	scale = scale > maxScale ? maxScale : scale;
	scale = scale < minScale ? minScale : scale;

	// compute the viewMat
	viewMat = glm::ortho<float>(-aspectRatio / scale, aspectRatio / scale, -1.f / scale, 1.f / scale, .1f, 23.f);
	
	// clear the color buffer and depth buffer (makes sure the trianges in front get rendered in front
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// temp -- will actually have player location
	Chunk::draw(character, deltaTime);
}


void MainWindow::input(float deltaTime)
{STACK
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


	character->setVelocity(vel);


	if (getKey(GLFW_KEY_Q))
	{
		character->setRotation(character->getRotation() + (-900.f * deltaTime));
	}

	if (getKey(GLFW_KEY_E))
	{
		character->setRotation(character->getRotation() + (900.f * deltaTime));
	}
	if (getKey(GLFW_KEY_P))
	{
		FATAL_ERR("YOU PRESSED P!", -12);
	}
}


void MainWindow::exit()
{STACK

	try{
//		CLHandler::exitCL();

	} catch (ENGException& e)
	{
		ENG_LOG(e.what());
		::exit(-12);
	}
}


void MainWindow::focus(int focused)
{STACK
	
}

void MainWindow::resize(double x, double y)
{STACK
	aspectRatio = float(x / y);
}