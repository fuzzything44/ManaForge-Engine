#include "OpenGLRendererConfig.h"

#include "OpenGLRenderer.h"
#include "OpenGLMaterial.h"
#include "OpenGLWindow.h"
#include "OpenGLTexture.h"


#include <Renderer.h>
#include <Helper.h>
#include <Logging.h>

#include <functional>
#include <algorithm>
#include "OpenGLTextureLibrary.h"

bool OpenGLRenderer::isDestroying = false;

OpenGLRenderer::OpenGLRenderer()
	: window(new OpenGLWindow())
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.2f, .2f, .2f, 1.f);
}

Window& OpenGLRenderer::getWindow()
{
	check(window);

	return *window;
}

const Window& OpenGLRenderer::getWindow() const
{
	check(window);

	return *window;
}

Model* OpenGLRenderer::newModel(const ModelData& params, MeshComponent* owner)
{
	return new OpenGLModel(params, owner, this);
}

Texture* OpenGLRenderer::newTexture(const std::string& name)
{
	return new OpenGLTexture(name);
}

TextureLibrary* OpenGLRenderer::newTextureLibrary(uint16 maxElems, uint16 individualSize)
{
	return new OpenGLTextureLibrary(maxElems, individualSize);
}

Material* OpenGLRenderer::newMaterial(const std::string& name)
{
	return new OpenGLMaterial(name);
}

void OpenGLRenderer::deleteTextureLibrary(TextureLibrary* library)
{
	delete library;
}
void OpenGLRenderer::deleteMaterial(Material* material)
{
	delete material;
}



void OpenGLRenderer::removeModel(Model* model)
{
	delete model;
}

bool OpenGLRenderer::update()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// call the draw function for all of the models
	std::for_each(models.begin(), models.end(), std::bind(&OpenGLModel::draw, std::placeholders::_1));

	window->swapBuffers();
	window->pollEvents();

	return !window->shouldClose();
}

void OpenGLRenderer::setCurrentCamera(CameraComponent* newCamera)
{
	currentCamera = newCamera;
}

void OpenGLRenderer::loadTextures(std::vector<std::string> /*textures*/)
{

}

OpenGLRenderer::~OpenGLRenderer()
{
	isDestroying = true;

	for (auto& elem : models)
	{
		delete elem;
	}


	OpenGLTexture::deleteAll();

	glFinish();

	delete window;

}

CameraComponent& OpenGLRenderer::getCurrentCamera()
{
	check(currentCamera);

	return *currentCamera;
}

const CameraComponent& OpenGLRenderer::getCurrentCamera() const
{
	check(currentCamera);

	return *currentCamera;
}

