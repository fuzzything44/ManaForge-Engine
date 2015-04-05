#define OpenGLRendererSource 1

#include "OpenGLRendererConfig.h"
#include <Renderer.h>
#include <ModuleManager.h>

class OpenGLRenderer : public Renderer
{ 
public:

	class OpenGLModel : public Model
	{
	public:
		OpenGLModel() 
			:locations(nullptr),
			UVs(nullptr),
			numVerts(0),
			numElems(0) { }

		OpenGLModel(float* locations, float* UVs, int numVerts, int numElems) 
			:locations(locations), 
			UVs(UVs),
			numVerts(numVerts),
			numElems(numElems){ }
	private:
		float* locations;
		float* UVs;
		int numVerts;
		int numElems;
	};

	virtual Model* newModel(float* locations, float* UVs, int numVerts, int numElems) override;

	OpenGLRenderer() { }

private:

};

Renderer::Model* OpenGLRenderer::newModel(float* locations, float* UVs, int numVerts, int numElems)
{
	return new OpenGLModel(locations, UVs, numVerts, numElems);
}

extern "C" OpenGLRendererPlugin_API void registerModule(ModuleManager& mm)
{
	mm.AddRenderer(new OpenGLRenderer());
}
