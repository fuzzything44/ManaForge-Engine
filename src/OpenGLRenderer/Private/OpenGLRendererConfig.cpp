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
	mm.registerClass<OpenGLRenderer>(MODULE_NAME);
	mm.addUpdateCallback([](float deltaTime)
	{
		return Runtime::get().renderer->update(deltaTime);
	});

	mm.addInitCallback([]()
	{
		return Runtime::get().renderer->init();
	});
}

extern "C" OpenGLRenderer_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}