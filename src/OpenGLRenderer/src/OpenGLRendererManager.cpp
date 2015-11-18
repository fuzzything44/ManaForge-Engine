#include "OpenGLRendererPCH.h"

#include "OpenGLRenderer/include/OpenGLRendererManager.h"

DLLEXPORT std::weak_ptr<OpenGLRendererManager_t> openGLRendererManager = {};

extern"C" DLLEXPORT void init(ModuleHandler&)
{
	openGLRendererManager = OpenGLRendererManager_t::factory(std::make_tuple(coreManager.lock().get()));
}


