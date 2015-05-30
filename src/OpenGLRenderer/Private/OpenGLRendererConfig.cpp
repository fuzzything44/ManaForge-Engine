#include "OpenGLRendererConfig.h"
#include "OpenGLRenderer.h"

#include <ModuleManager.h>

extern "C" OpenGLRenderer_API void registerModule(ModuleManager& mm)
{
	mm.setRenderer(new OpenGLRenderer());
	mm.addUpdateCallback(
		std::bind(&OpenGLRenderer::update,
		static_cast<OpenGLRenderer*>(&mm.getRenderer())));
}

extern "C" OpenGLRenderer_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}