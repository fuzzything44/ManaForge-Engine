#pragma once
#include "Engine.h"

#include "Transform.h"

// forward declarations
class MeshComponent;
class Material;

struct ModelBounds
{
	ModelBounds(
		const vec2& lowerLeft  = vec2{ 0.f },
		const vec2& upperLeft  = vec2{ 0.f },
		const vec2& lowerRight = vec2{ 0.f },
		const vec2& upperRight = vec2{ 0.f })

		:lowerLeft(lowerLeft),
		upperLeft(upperLeft),
		lowerRight(lowerRight),
		upperRight(upperRight)
	{ }

	vec2 lowerLeft;
	vec2 upperLeft;
	vec2 lowerRight;
	vec2 upperRight;
};

struct ModelData
{
	ModelData(
		vec2* vertexLocations = nullptr,
		vec2* UVs = nullptr,
		uvec3* triangles = nullptr,
		uint32 numVerts = 0,
		uint32 numTriangles = 0,
		Material* material = nullptr,
		const ModelBounds& bounds = ModelBounds{})
		
		:vertexLocations(vertexLocations),
		UVs(UVs),
		triangles(triangles),
		numVerts(numVerts),
		numTriangles(numTriangles),
		material(material),
		bounds(bounds)
	{

	}

	vec2* vertexLocations;
	vec2* UVs;
	uvec3* triangles;
	uint32 numVerts;
	uint32 numTriangles;
	Material* material;
	ModelBounds bounds;
};

class Model
{
public:
	virtual ~Model()
	{
	}

	Model() { }
	
	virtual MeshComponent* getOwnerComponent() = 0;


};
