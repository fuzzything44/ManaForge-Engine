#pragma once
#include "Engine.h"
#include "Model.h"

#include <vector>

/// <summary> Width of the chunk.</summary>
static const uint32 CHUNK_WIDTH = 50;

// forward declarations
class Actor;
class World;

/// <summary> A chunk.</summary>
class Chunk
{
public:

	// freind the actor class so we can share private methods
	friend Actor;
	
	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	vec2 ENGINE_API getLocation();

	/// <summary> Constructor.</summary>
	///
	/// <param name="locationIn"> The location in.</param>
	ENGINE_API explicit Chunk(ivec2 locationIn = ivec2(0, 0), Model* model = nullptr);
	ENGINE_API explicit Chunk(ivec2 locationIn, vec2* locations, vec2* UVs, uint32* elems, uint32 numVerts, uint32 numElems);

protected:

	Model* model;

	/// <summary> The actors in the chunk.</summary>
	std::vector<Actor*> actors;

	/// <summary> The location. in world units</summary>
	vec2 location;

	// the world that this chunk belongs to
	World* world;

};

#include "Actor.h"
#include "World.h"
