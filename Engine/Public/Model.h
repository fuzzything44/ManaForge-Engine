#pragma once

#include "Engine.h"
#include "Transform.h"


class Model
{
public:
	virtual ~Model()
	{
	}

	Model() { }
	Model(Transform /*location*/, float* /*locations*/, float* /*UVs*/, uint32 /*numVerts*/, uint32 /*numElems*/) { }

	virtual Transform getTransform() const = 0;
	virtual vec2 getLocation() const = 0;
	virtual float getRotation() const = 0;
	virtual vec2 getScale() const = 0;

	virtual void setTransform(Transform newTransform) = 0;
	virtual void setLocation(vec2 newLocation) = 0;
	virtual void setRotation(float newRotation) = 0;
	virtual void setScale(vec2 newScale) = 0;

	virtual void addRelativeTransform(Transform add) = 0;
	virtual void addRelativeLocation(vec2 locToAdd) = 0;
	virtual void addRelativeRotation(float rotToAdd) = 0;
	virtual void addRelativeScale(vec2 scaleToAdd) = 0;


};
