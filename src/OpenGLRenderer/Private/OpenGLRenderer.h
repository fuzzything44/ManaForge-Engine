#pragma once
#include "Renderer.h"
#include <list>
#include <vector>
#include "OpenGLWindow.h"

class OpenGLModel;

class OpenGLRenderer : public Renderer
{

	friend class OpenGLModel;

public:


	OpenGLRenderer();

	virtual Window& getWindow() override;
	const Window& getWindow() const override;

	virtual Model* newModel(const ModelData& params, MeshComponent* owner) override;
	virtual Texture* newTexture(const std::string& name) override;
	virtual Material* newMaterial() override;

	/// <summary> Renders the next frame. </summary>
	virtual bool update() override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent* newCamera) override;

	void loadTextures(std::vector<std::string> textures) override;

	virtual ~OpenGLRenderer() override;

	CameraComponent& getCurrentCamera() override;
	const CameraComponent& getCurrentCamera() const override;


private:
	
	OpenGLWindow* window;


	CameraComponent* currentCamera;

	// doubley linked list of the models
	std::list<OpenGLModel*> models;
};


#include "OpenGLModel.h"