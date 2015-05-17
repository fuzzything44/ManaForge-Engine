#pragma once

#include <Engine.h>
#include <Model.h>
#include "OpenGLMaterial.h"

class OpenGLModel : public Model
{
public:

	friend class OpenGLRenderer;

	explicit OpenGLModel(ModelData data, OpenGLRenderer* renderer);

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

	virtual ~OpenGLModel();



protected:

	void draw();

	vec2* locations;
	vec2* UVs;
	uint32* elems;
	uint32 numVerts;
	uint32 numTris;

	uint32 vertexArray;
	uint32 vertexLocationBuffer;
	uint32 texCoordBuffer;
	uint32 elemBuffer;

	GLint viewMatUniformLocation;

	OpenGLRenderer* renderer;

	Transform trans;

	Component* parent;

	OpenGLMaterial* material;

};