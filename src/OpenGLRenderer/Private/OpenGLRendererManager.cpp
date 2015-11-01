#include "OpenGLRendererPCH.h"

#include "OpenGLRendererManager.h"

std::weak_ptr<OpenGLRendererManager_t> openGLRendererManager = {};

extern"C" __declspec(dllexport) void init()
{
	openGLRendererManager = OpenGLRendererManager_t::factory(std::make_tuple(coreManager.lock().get()));
}


