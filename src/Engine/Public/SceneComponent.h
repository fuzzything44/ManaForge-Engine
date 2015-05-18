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
	explicit ENGINE_API SceneComponent(Actor* owner = nullptr, Transform trans = Transform{});

	ENGINE_API virtual ~SceneComponent() override;

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	inline ENGINE_API vec2 getLocation() const;

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	inline ENGINE_API vec2 getScale() const;

	/// <summary> Gets the rotation.</summary>
	///
	/// <returns> The rotation.</returns>
	inline ENGINE_API float getRotation() const;

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

inline SceneComponent::SceneComponent(Actor* owner, Transform trans):Component(owner),
                                                                     trans(trans)
{
		
}

inline SceneComponent::~SceneComponent()
{

}

inline vec2 SceneComponent::getLocation() const
{
	return trans.location;
}


inline vec2 SceneComponent::getScale() const
{
	return trans.scale;
}


inline float SceneComponent::getRotation() const
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

