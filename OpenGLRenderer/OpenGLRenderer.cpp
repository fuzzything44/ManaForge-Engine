#include "OpenGLRendererConfig.h"
#include <OpenGLRenderer.h>
#include <Renderer.h>
#include <ModuleManager.h>
#include <Helper.h>

Model* OpenGLRenderer::newModel(float* locations, float* UVs, int numVerts, int numElems)
{
	return new OpenGLModel(locations, UVs, numVerts, numElems);
}

void OpenGLRenderer::init()
{
	
}

void OpenGLRenderer::update()
{
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