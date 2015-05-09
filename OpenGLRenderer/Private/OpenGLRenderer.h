#pragma once
#include "Renderer.h"
#include <forward_list>
#include <vector>

class OpenGLModel;

class OpenGLRenderer : public Renderer
{
	friend class OpenGLModel;

public:

	uint32 loadShaderProgram(std::string program);

	virtual Model* newModel(vec2* locations, vec2* UVs, uint32* elems, uint32 numVerts, uint32 numElems) override;

	virtual Material* newMaterial() override;

	/// <summary>
	virtual void init() override;

	/// <summary> Renders the next frame. </summary>
	virtual void update() override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent* newCamera) override;

	void loadTextures(std::vector<std::string> textures) override;

	virtual ~OpenGLRenderer() override;

	CameraComponent* getCurrentCamera() override;

	OpenGLRenderer() { }



private:
	CameraComponent* currentCamera;

	std::forward_list<OpenGLModel*> models;
};


#include "OpenGLModel.h"