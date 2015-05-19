#pragma once

#include "Engine.h"
#include "Transform.h"

// forward declarations
class MeshComponent;
class Material;

struct ModelData
{
	ModelData(
		const Transform& trans = Transform{},
		vec2* vertexLocations = nullptr,
		vec2* UVs = nullptr,
		uvec3* triangles = nullptr,
		uint32 numVerts = 0,
		uint32 numTriangles = 0,
		Material* material = nullptr)
		
		:trans(trans),
		vertexLocations(vertexLocations),
		UVs(UVs),
		triangles(triangles),
		numVerts(numVerts),
		numTriangles(numTriangles),
		material(material)
	{

	}

	Transform trans;
	vec2* vertexLocations;
	vec2* UVs;
	uvec3* triangles;
	uint32 numVerts;
	uint32 numTriangles;
	Material* material;
};

class Model
{
public:
	virtual ~Model()
	{
	}

	Model() { }

	virtual Transform getTransform() const = 0;
	virtual vec2 getLocation() const = 0;
	virtual float getRotation() const = 0;
	virtual vec2 getScale() const = 0;

	virtual MeshComponent* getOwnerComponent() = 0;

	virtual void setTransform(Transform newTransform) = 0;
	virtual void setLocation(vec2 newLocation) = 0;
	virtual void setRotation(float newRotation) = 0;
	virtual void setScale(vec2 newScale) = 0;

	virtual void addRelativeTransform(Transform add) = 0;
	virtual void addRelativeLocation(vec2 locToAdd) = 0;
	virtual void addRelativeRotation(float rotToAdd) = 0;
	virtual void addRelativeScale(vec2 scaleToAdd) = 0;


};
