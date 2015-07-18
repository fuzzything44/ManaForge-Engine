#pragma once
#include "Renderer.h"

#include <list>
#include <vector>
#include <unordered_map>
#include <thread>


class OpenGLModel;
class OpenGLMaterialInstance;
class OpenGLTexture;
class OpenGLWindow;
class OpenGLMaterialSource;

class OpenGLRenderer : public Renderer
{

	friend class OpenGLModel;

public:


	OpenGLRenderer();

	virtual ~OpenGLRenderer() override;

	virtual void init() override;

	void showLoadingImage();

	virtual Window& getWindow() override;
	const Window& getWindow() const override;

	virtual std::unique_ptr<Model> newModel() override;
	virtual std::shared_ptr<Texture> getTexture(const path_t& name) override;
	virtual std::shared_ptr<MaterialSource> getMaterialSource(const path_t& name) override;
	virtual std::unique_ptr<TextureLibrary> newTextureLibrary() override;
	virtual std::unique_ptr<MaterialInstance> newMaterial(std::shared_ptr<MaterialSource> source) override;
	virtual std::unique_ptr<ModelData> newModelData() override;

	/// <summary> Renders the next frame. </summary>
	virtual bool update(float deltaTime) override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent& newCamera) override;


	CameraComponent& getCurrentCamera() override;
	const CameraComponent& getCurrentCamera() const override;

	virtual void drawDebugOutlinePolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugLine(vec2* locs, uint32 numLocs, Color color) override;
	virtual void drawDebugSolidPolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugOutlineCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSolidCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSegment(vec2 p1, vec2 p2, Color color) override;

private:
	

	std::unique_ptr<OpenGLWindow> window;

	std::unique_ptr<OpenGLMaterialInstance> debugDraw;

	CameraComponent* currentCamera;

	// doubley linked list of the models
	std::list<OpenGLModel*> models;
	std::unordered_map<path_t, std::weak_ptr<OpenGLTexture> > textures;
	std::unordered_map<path_t, std::weak_ptr<OpenGLMaterialSource> > matSources;


};
