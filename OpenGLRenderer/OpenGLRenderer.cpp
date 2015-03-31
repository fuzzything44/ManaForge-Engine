#define OpenGLRendererSource 1

#include "OpenGLRendererConfig.h"
#include <Renderer.h>
#include <ModuleManager.h>

class OpenGLRenderer : public Renderer
{ 
public:

	OpenGLRenderer() { }
};

extern "C" OpenGLRendererPlugin_API void registerModule(ModuleManager& mm)
{
	mm.AddRenderer(new OpenGLRenderer());
}
