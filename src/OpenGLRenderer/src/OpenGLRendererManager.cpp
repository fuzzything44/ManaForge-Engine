#include "OpenGLRendererPCH.h"

#include "OpenGLRenderer/include/OpenGLRendererManager.h"

__declspec(dllexport) std::weak_ptr<OpenGLRendererManager_t> openGLRendererManager = {};

extern"C" __declspec(dllexport) void init(ModuleHandler&)
{
	openGLRendererManager = OpenGLRendererManager_t::factory(std::make_tuple(coreManager.lock().get()));
}


