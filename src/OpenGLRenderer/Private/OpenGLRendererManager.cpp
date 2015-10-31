#include "OpenGLRendererPCH.h"

#include "OpenGLRendererManager.h"

std::shared_ptr<OpenGLRendererManager_t> openGLRendererManager = nullptr;

extern"C" __declspec(dllexport) void init()
{
	openGLRendererManager = OpenGLRendererManager_t::factory(std::make_tuple(coreManager));
}


