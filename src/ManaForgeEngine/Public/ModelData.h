#pragma once

#include "Engine.h"

class ModelData
{
public:
	virtual ~ModelData() = default;

	virtual void init(const vec2* vertLocs, const vec2* UVs, uint32 numVerts, const uvec3* elems, uint32 numElems) = 0;
	virtual bool isInitialized() = 0;
	// TODO:
	// virtual void init(const path_t& path)
};
