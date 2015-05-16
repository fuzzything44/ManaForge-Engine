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

	virtual Model* newModel(const vec2* vertexLocations, const vec2* UVs, const uvec3* tris, uint32 numVerts, uint32 numTris) override;

	virtual Material* newMaterial() override;

	/// <summary> Renders the next frame. </summary>
	virtual bool update() override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent& newCamera) override;

	void loadTextures(std::vector<std::string> textures) override;

	virtual ~OpenGLRenderer() override;

	CameraComponent& getCurrentCamera() override;
	const CameraComponent& getCurrentCamera() const override;


private:
	
	OpenGLWindow* window;

	uint32 loadShaderProgram(std::string program);
	int32 loadDDS(const std::string& filename);
	void appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath);
	uint32 allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath);


	CameraComponent* currentCamera;

	// doubley linked list of the models
	std::list<OpenGLModel*> models;
};


#include "OpenGLModel.h"