#pragma once
#include "Renderer.h"
#include <list>
#include <vector>
#include "OpenGLWindow.h"

class OpenGLModel;
class OpenGLMaterial;
class OpenGLTexture;

class OpenGLRenderer : public Renderer
{

	friend class OpenGLModel;

public:


	OpenGLRenderer();

	virtual ~OpenGLRenderer() override;

	virtual Window& getWindow() override;
	const Window& getWindow() const override;

	virtual std::unique_ptr<Model> newModel(const ModelData& params, MeshComponent* owner) override;
	virtual std::shared_ptr<Texture> newTexture(const std::string& name) override;
	virtual TextureLibrary* newTextureLibrary(uint16, uint16) override;
	virtual Material* newMaterial(const std::string& name) override;

	virtual void deleteTextureLibrary(TextureLibrary* library) override;
	virtual void deleteMaterial(Material* material) override;

	virtual void removeModel(Model* model) override;

	/// <summary> Renders the next frame. </summary>
	virtual bool update() override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent* newCamera) override;

	void loadTextures(std::vector<std::string> textures) override;


	CameraComponent& getCurrentCamera() override;
	const CameraComponent& getCurrentCamera() const override;

	virtual void drawDebugOutlinePolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugLine(vec2* locs, uint32 numLocs, Color color) override;
	virtual void drawDebugSolidPolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugOutlineCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSolidCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSegment(vec2 p1, vec2 p2, Color color) override;

private:
	

	OpenGLWindow* window;

	OpenGLMaterial* debugDraw;

	CameraComponent* currentCamera;

	// doubley linked list of the models
	std::list<OpenGLModel*> models;
	std::map<std::string, std::shared_ptr<OpenGLTexture> > textures;

	static bool isDestroying;
};


#include "OpenGLModel.h"