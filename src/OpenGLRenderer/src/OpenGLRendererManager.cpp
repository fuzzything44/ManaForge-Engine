#include "OpenGLRendererPCH.h"

#include "OpenGLRendererManager.h"

DLLEXPORT OpenGLRendererManager_t* openGLRendererManager = nullptr;

extern"C" DLLEXPORT void init(ModuleHandler&)
{
	openGLRendererManager = new OpenGLRendererManager_t(boost::hana::make_tuple(coreManager));
}

extern"C" DLLEXPORT void cleanup(ModuleHandler& handler)
{
	delete openGLRendererManager;
	openGLRendererManager = nullptr;
}

