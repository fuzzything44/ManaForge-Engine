#pragma once

#include "Component.h"
#include "Transform.h"

/// <summary> a component that has a transform </summary>
class SceneComponent : public Component
{
public:

	/// <summary> Default constructor. </summary>
	/// <param name="owner"> The owner actor. Will usaually be this </param>
	/// <param name="trans"> The transform, relative to the owner actor </param>
	explicit ENGINE_API SceneComponent(Actor* owner = nullptr, Transform trans = Transform{})
		:Component(owner),
		trans(trans)
	{
		
	}

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	inline ENGINE_API vec2 getLocation();

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	inline ENGINE_API vec2 getScale();

	/// <summary> Gets the rotation.</summary>
	///
	/// <returns> The rotation.</returns>
	inline ENGINE_API float getRotation();

	/// <summary> Sets a location.</summary>
	///
	/// <param name="newLoc"> The new location.</param>
	inline ENGINE_API void setLocation(vec2 newLoc);

	/// <summary> Sets a size.</summary>
	///
	/// <param name="newSize"> Size of the new.</param>
	inline ENGINE_API void setScale(vec2 newScale);

	/// <summary> Sets a rotation.</summary>
	///
	/// <param name="newRot"> The new rot.</param>
	inline ENGINE_API void setRotation(float newRot);


protected:
	Transform trans;
};


///////////////////////
///// INLINE DEFINITIONS
///////////////////////

inline vec2 SceneComponent::getLocation()
{
	return trans.location;
}


inline vec2 SceneComponent::getScale()
{
	return trans.scale;
}


inline float SceneComponent::getRotation()
{
	return trans.rotation;
}

inline void SceneComponent::setLocation(vec2 newLoc)
{
	trans.location = newLoc;
}


inline void SceneComponent::setScale(vec2 newScale)
{
	trans.scale = newScale;
}


inline void SceneComponent::setRotation(float newRot)
{
	trans.rotation = newRot;
}

