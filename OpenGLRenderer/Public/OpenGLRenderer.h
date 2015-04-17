#pragma once
#include "Renderer.h"

class OpenGLRenderer : public Renderer
{


public:

	virtual Model* newModel(float* locations, float* UVs, int numVerts, int numElems) override;

	/// <summary>
	virtual void init() override;

	/// <summary> Renders the next frame. </summary>
	virtual void update() override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(Camera* newCamera) override;

	void loadTextures(std::vector<std::string> textures) override;
	~OpenGLRenderer() override;

	Camera* getCurrentCamera() override;

	OpenGLRenderer() { }



private:
	Camera* currentCamera;
};


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

	virtual Transform getTransform() const override;

	virtual vec2 getLocation() const override;
	virtual float getRotation() const override;
	virtual vec2 getScale() const override;

	virtual void setTransform(Transform newTransform) override;
	virtual void setLocation(vec2 newLocation) override;
	virtual void setRotation(float newRotation) override;
	virtual void setScale(vec2 newScale) override;


	virtual void addRelativeTransform(Transform add) override;
	virtual void addRelativeLocation(vec2 locToAdd) override;
	virtual void addRelativeRotation(float rotToAdd) override;
	virtual void addRelativeScale(vec2 scaleToAdd) override;


private:
	float* locations;
	float* UVs;
	int numVerts;
	int numElems;

	Transform trans;

	Actor* parent;

};