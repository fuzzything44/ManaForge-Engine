#include "OpenGLRendererPCH.h"

#include "OpenGLRenderer/include/OpenGLRendererManager.h"

DLLEXPORT OpenGLRendererManager_t* openGLRendererManager = nullptr;

extern"C" DLLEXPORT void init(ModuleHandler&)
{
	openGLRendererManager = OpenGLRendererManager_t::factory(std::make_tuple(coreManager));
}

extern"C" DLLEXPORT void cleanup(ModuleHandler& handler)
{
	delete openGLRendererManager;
	openGLRendererManager = nullptr;
}

