#pragma once 

#include "Engine.h"
#include "Model.h"
#include "CameraComponent.h"
#include "Material.h"

class Renderer 
{
public:

	

	/// <summary> Default constructor.</summary>
	Renderer() { };

	/// <summary> Initialises the Renderer.</summary>
	///
	/// <param name="textures"> The textures.</param>
	virtual void init() = 0;
	
	/// <summary> Renders the next frame. </summary>
	virtual void update() = 0;
	
	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent* newCamera) = 0;

	/// <summary> Gets the camera.</summary>
	///
	/// <returns> null if it fails, else the camera.</returns>
	virtual CameraComponent* getCurrentCamera() = 0;

	/// <summary> Creates a new model.</summary>
	///
	/// <returns> null if it fails, else a Model*.</returns>
	virtual Model* newModel(vec2* locations, vec2* UVs, uint32* elems, uint32 numVerts, uint32 numElems) = 0;

	virtual Material* newMaterial() = 0;

	/// <summary> Loads the textures.</summary>
	///
	/// <param name="textures"> The textures.</param>
	virtual void loadTextures(std::vector<std::string> textures) = 0;

	/// <summary> Destructor.</summary>
	virtual ~Renderer() { }

};