#pragma once 

#include <boost/core/noncopyable.hpp>

#include "Engine.h"
#include "WindowProps.h"

#include <vector>

// forward declarations
class Material;
class CameraComponent;
class Window;
class Model;

// a non - copyable abstract class that handles rendering
class Renderer :
	boost::noncopyable
{
public:

	

	/// <summary> Default constructor.</summary>
	Renderer() { }

	/// <summary> Renders the next frame. </summary>
	virtual bool update() = 0;
	
	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent& newCamera) = 0;

	/// <summary> Gets the camera.</summary>
	///
	/// <returns> null if it fails, else the camera.</returns>
	virtual CameraComponent& getCurrentCamera() = 0;
	/// <summary> Gets the camera from a const object.</summary>
	///
	/// <returns> null if it fails, else the camera.</returns>
	virtual const CameraComponent& getCurrentCamera() const = 0;

	/// <summary> Creates a new model.</summary>
	///
	/// <returns> null if it fails, else a Model*.</returns>
	virtual Model* newModel(const vec2* VertexLocations, const vec2* UVs, const uvec3* tris, uint32 numVerts, uint32 numTris) = 0;

	virtual Material* newMaterial() = 0;

	/// <summary> Loads the textures.</summary>
	///
	/// <param name="textures"> The textures.</param>
	virtual void loadTextures(std::vector<std::string> textures) = 0;

	/// <summary> Destructor.</summary>
	virtual ~Renderer() { }

	// gets the window
	virtual Window& getWindow() = 0;
	// and the const version
	virtual const Window& getWindow() const = 0;

};

#include "Window.h"
#include "CameraComponent.h"
#include "Model.h"
#include "Material.h"
