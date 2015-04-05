#pragma once 

#include "Engine.h"
#include "Actor.h"

class Renderer 
{
public:

	class Model
	{
	public:
		Model() { }
		Model(float* locations, float* UVs, int numVerts, int numElems) { }

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


	private:

		Transform transform;

	};

	/// <summary> Default constructor.</summary>
	Renderer(){ }

	/// <summary> Creates a new model.</summary>
	///
	/// <returns> null if it fails, else a Model*.</returns>
	virtual Model* newModel(float* locations, float* UVs, int numVerts, int numElems) = 0;

	virtual ~Renderer() { }

};
