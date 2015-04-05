#pragma once 

#include "Engine.h"

class Renderer 
{
public:

	class Model
	{
	public:
		Model() { }
		Model(float* locations, float* UVs, int numVerts, int numElems) { }
	};

	/// <summary> Default constructor.</summary>
	Renderer(){ }

	/// <summary> Creates a new model.</summary>
	///
	/// <returns> null if it fails, else a Model*.</returns>
	virtual Model* newModel(float* locations, float* UVs, int numVerts, int numElems) = 0;

	virtual ~Renderer() { }

};
