#pragma once

#include <Engine.h>
#include <Model.h>
#include <Actor.h>

class OpenGLModel : public Model
{
public:

	friend class OpenGLRenderer;

	explicit OpenGLModel(
		vec2* locations = nullptr,
		vec2* UVs = nullptr,
		uint32* elems = nullptr,
		uint32 numVerts = 0,
		uint32 numElems = 0,
		OpenGLRenderer* renderer = nullptr);

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
	uint32 numElems;

	uint32 vertexArray;
	uint32 vertexLocationBuffer;
	uint32 texCoordBuffer;
	uint32 elemBuffer;

	static int32 viewMatUniformLoc;
	static int32 texUniformLoc;
	static uint32 program;

	uint32 texture;

	OpenGLRenderer* renderer;

	Transform trans;

	Actor* parent;

};