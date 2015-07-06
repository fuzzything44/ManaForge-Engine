#include "OpenGLRendererConfig.h"
#include "OpenGLRenderer.h"

#include <Helper.h>
#include <ModuleManager.h>

// link to required libs
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "OpenGL32.lib")

#ifdef _DEBUG
#	pragma comment(lib, "glew32d.lib")
#else 
#	pragma comment(lib, "glew32.lib")
#endif



extern "C" OpenGLRenderer_API void registerModule(ModuleManager& mm)
{
	mm.setRenderer(std::make_unique<OpenGLRenderer>());

	mm.addUpdateCallback(
		std::bind(&OpenGLRenderer::update,
		static_cast<OpenGLRenderer*>(&mm.getRenderer())));
}

extern "C" OpenGLRenderer_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}