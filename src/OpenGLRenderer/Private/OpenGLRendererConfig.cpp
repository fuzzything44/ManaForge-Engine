#include "OpenGLRendererPCH.h"

#include "OpenGLRendererConfig.h"
#include "OpenGLRenderer.h"

#include <Helper.h>
#include <ModuleManager.h>
#include <Runtime.h>

// link to required libs
#pragma comment(lib, "OpenGL32.lib")
#	pragma comment(lib, "glfw3.lib")

#ifdef _DEBUG
#	pragma comment(lib, "glew32d.lib")
#else 
#	pragma comment(lib, "glew32.lib")
#endif



extern "C" OpenGLRenderer_API void registerModule(ModuleManager& mm)
{
	mm.setRenderer(std::make_unique<OpenGLRenderer>());

	mm.addUpdateCallback([](float deltaTime)
	{
		return Runtime::get().moduleManager.getRenderer().update(deltaTime);
	});

	mm.addInitCallback([]()
	{
		return Runtime::get().moduleManager.getRenderer().init();
	});
}

extern "C" OpenGLRenderer_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}