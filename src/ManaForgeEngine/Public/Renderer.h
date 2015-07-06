#pragma once 
#include "Engine.h"

#include "WindowProps.h"
#include "Color.h"

#include <vector>

#include <boost/core/noncopyable.hpp>


// forward declarations
class MeshComponent;
class Texture;
class Material;
class CameraComponent;
class Window;
class Model;
class TextureLibrary;
struct ModelData;

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
	virtual std::unique_ptr<Model> newModel(const ModelData& data, MeshComponent& owner) = 0;
	virtual std::shared_ptr<Texture> newTexture(const std::string& name) = 0;
	virtual std::unique_ptr<TextureLibrary> newTextureLibrary(uint16, uint16) = 0;
	virtual std::unique_ptr<Material> newMaterial(const std::string& name) = 0;


	/// <summary> Destructor.</summary>
	virtual ~Renderer() { }

	// gets the window
	virtual Window& getWindow() = 0;
	// and the const version
	virtual const Window& getWindow() const = 0;


	virtual void drawDebugOutlinePolygon(vec2* verts, uint32 numVerts, Color color) = 0;
	virtual void drawDebugLine(vec2* locs, uint32 numLocs, Color color) = 0;	
	virtual void drawDebugSolidPolygon(vec2* verts, uint32 numVerts, Color color) = 0;
	virtual void drawDebugOutlineCircle(vec2 center, float radius, Color color) = 0;
	virtual void drawDebugSolidCircle(vec2 center, float radius, Color color) = 0;
	virtual void drawDebugSegment(vec2 p1, vec2 p2, Color color) = 0;

};
