#pragma once
#include "Engine.h"

#include "Color.h"

#include <vector>

#include <boost/core/noncopyable.hpp>

// forward declarations
class MeshComponent;
class Texture;
class MaterialInstance;
class CameraComponent;
class WindowWidget;
class TextureLibrary;
class ModelData;
class MaterialSource;
class Font;
class Model;
class Widget;

namespace MFUI
{
	class TextBoxWidget;
}


// a non - copyable abstract class that handles rendering
class Renderer : boost::noncopyable
{
public:
	/// <summary> Destructor.</summary>
	virtual ~Renderer() = default;

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

	virtual std::unique_ptr<Model, void (*)(Model*)> newModel(uint8 renderOrder /* = 0*/) = 0;
	virtual std::unique_ptr<MFUI::TextBoxWidget> newTextBoxWidget(Widget* owner) = 0;
	virtual Font* getFont(const path_t& name) = 0;
	virtual Texture* getTexture(const path_t& name) = 0;
	virtual MaterialSource* getMaterialSource(const path_t& name) = 0;
	virtual std::unique_ptr<TextureLibrary> newTextureLibrary() = 0;
	virtual std::unique_ptr<MaterialInstance> newMaterialInstance(MaterialSource* source) = 0;
	virtual std::shared_ptr<ModelData> newModelData(const std::string& name) = 0;
	virtual std::unique_ptr<ModelData> newModelData() = 0;

	virtual void deleteModel(Model* model) = 0;

	// gets the window
	virtual WindowWidget* getWindow() = 0;
	// and the const version
	virtual const WindowWidget* getWindow() const = 0;

	virtual void drawDebugOutlinePolygon(vec2* verts, uint32 numVerts, Color color) = 0;
	virtual void drawDebugLine(vec2* locs, uint32 numLocs, Color color) = 0;
	virtual void drawDebugSolidPolygon(vec2* verts, uint32 numVerts, Color color) = 0;
	virtual void drawDebugOutlineCircle(vec2 center, float radius, Color color) = 0;
	virtual void drawDebugSolidCircle(vec2 center, float radius, Color color) = 0;
	virtual void drawDebugSegment(vec2 p1, vec2 p2, Color color) = 0;
};
