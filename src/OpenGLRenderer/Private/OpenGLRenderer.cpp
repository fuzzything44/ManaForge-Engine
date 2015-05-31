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

OpenGLRenderer::OpenGLRenderer()
	: window(new OpenGLWindow())
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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


bool OpenGLRenderer::update()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->getWindowProps().size.x, window->getWindowProps().size.y);
	glClearColor(.2f, .2f, .2f, 1.f);

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

