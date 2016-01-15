#include "OpenGLRendererManager.h"

#include <ModuleHandler.h>

DLLEXPORT OpenGLRendererManager_t* openGLRendererManager = nullptr;

extern "C" DLLEXPORT void mf_init_module(ModuleHandler&)
{
	openGLRendererManager = new OpenGLRendererManager_t(boost::hana::make_tuple(coreManager));
}

extern "C" DLLEXPORT void mf_cleanup_module(ModuleHandler& handler)
{
	delete openGLRendererManager;
	openGLRendererManager = nullptr;
}