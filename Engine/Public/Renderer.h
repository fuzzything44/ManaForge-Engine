#pragma once 

#include <boost/core/noncopyable.hpp>

#include "Engine.h"
#include "Model.h"
#include "Material.h"

#include <vector>

// forward declarations
class CameraComponent;

// a non - copyable abstract class that handles rendering
class Renderer :
	boost::noncopyable
{
public:

	

	/// <summary> Default constructor.</summary>
	Renderer() { };

	/// <summary> Renders the next frame. </summary>
	virtual bool update() = 0;
	
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


#include "CameraComponent.h"
