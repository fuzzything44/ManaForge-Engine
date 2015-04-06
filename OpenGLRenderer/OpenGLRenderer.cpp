#define OpenGLRendererSource 1

#include "OpenGLRendererConfig.h"
#include <Renderer.h>
#include <ModuleManager.h>
#include <Actor.h>

class OpenGLRenderer : public Renderer
{

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

		Actor* parent;

	};

public:

	virtual Model* newModel(float* locations, float* UVs, int numVerts, int numElems) override;

	/// <summary>
	virtual void init(std::string textures) override;

	/// <summary> Renders the next frame. </summary>
	virtual void update() override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCamera(Camera* newCamera) override;

	void loadTextures(std::vector<std::string> textures) override;
	~OpenGLRenderer() override;

	Camera* getCamera() override;

	OpenGLRenderer() { }



private:
	Camera* currentCamera;
};

Renderer::Model* OpenGLRenderer::newModel(float* locations, float* UVs, int numVerts, int numElems)
{
	return new OpenGLModel(locations, UVs, numVerts, numElems);
}

void OpenGLRenderer::init(std::string textures)
{
	
}

void OpenGLRenderer::update()
{
}

void OpenGLRenderer::setCamera(Camera* newCamera)
{
	check(newCamera);

	currentCamera = newCamera;

}

void OpenGLRenderer::loadTextures(std::vector<std::string> textures)
{

}

OpenGLRenderer::~OpenGLRenderer()
{

}

Camera* OpenGLRenderer::getCamera()
{
	return currentCamera;
}

extern "C" OpenGLRendererPlugin_API void registerModule(ModuleManager& mm)
{
	mm.AddRenderer(new OpenGLRenderer());
}
