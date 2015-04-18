#include "OpenGLRendererConfig.h"
#include <OpenGLRenderer.h>
#include <Renderer.h>
#include <ModuleManager.h>
#include <Helper.h>
#include <functional>

Model* OpenGLRenderer::newModel(vec2* locations, vec2* UVs, uint32* elems, uint32 numVerts, uint32 numElems)
{
	return new OpenGLModel(locations, UVs, elems, numVerts, numElems, this);
}

void OpenGLRenderer::init()
{
	
}

void OpenGLRenderer::update()
{
	// TODO: collision etc

	// call the draw function for all of the models
	std::for_each(models.begin(), models.end(), std::bind(&OpenGLModel::draw, std::placeholders::_1));
}

void OpenGLRenderer::setCurrentCamera(CameraComponent* newCamera)
{
	check(newCamera);

	currentCamera = newCamera;

}

void OpenGLRenderer::loadTextures(std::vector<std::string> /*textures*/)
{

}

OpenGLRenderer::~OpenGLRenderer()
{

}

CameraComponent* OpenGLRenderer::getCurrentCamera()
{
	return currentCamera;
}

extern "C" OpenGLRendererPlugin_API void registerModule(ModuleManager& mm)
{
	mm.addRenderer(new OpenGLRenderer());
}

extern "C" OpenGLRendererPlugin_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}