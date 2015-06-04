#pragma once

#include "Component.h"
#include "Transform.h"
#include <glm/detail/type_mat.hpp>

/// <summary> a component that has a transform </summary>
class SceneComponent : public Component
{
public:

	/// <summary> Default constructor. </summary>
	/// <param name="owner"> The owner actor. Will usaually be this </param>
	/// <param name="trans"> The transform, relative to the owner actor </param>
	explicit SceneComponent(Actor* owner = nullptr, Transform trans = Transform{});

	virtual ~SceneComponent() override;

	ENGINE_API inline Transform getLocalTransform() const;
	ENGINE_API inline Transform getWorldTransform() const;

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	ENGINE_API inline vec2 getLocalLocation() const;

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	ENGINE_API inline vec2 getWorldLocation() const;

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	ENGINE_API inline vec2 getScale() const;

	/// <summary> Gets the rotation.</summary>
	///
	/// <returns> The rotation.</returns>
	ENGINE_API inline float getLocalRotation() const;

	/// <summary> Gets the rotation.</summary>
	///
	/// <returns> The rotation.</returns>
	ENGINE_API inline float getWorldRotation() const;

	/// <summary> Sets a location.</summary>
	///
	/// <param name="newLoc"> The new location.</param>
	ENGINE_API inline void setLocation(vec2 newLoc);

	/// <summary> Sets a size.</summary>
	///
	/// <param name="newSize"> Size of the new.</param>
	ENGINE_API inline void setScale(vec2 newScale);

	/// <summary> Sets a rotation.</summary>
	///
	/// <param name="newRot"> The new rot.</param>
	ENGINE_API inline void setRotation(float newRot);


protected:
	Transform trans;
};


///////////////////////
///// INLINE DEFINITIONS
///////////////////////
#include "Actor.h"

inline SceneComponent::SceneComponent(Actor* owner, Transform trans)
	: Component(owner),
	trans(trans)
{
		
}

inline SceneComponent::~SceneComponent()
{

}

inline Transform SceneComponent::getLocalTransform() const
{
	return trans;
}

inline Transform SceneComponent::getWorldTransform() const
{
	
	if (owner)
	{

		vec2 start = trans.location;

		start.x *= cos(owner->getWorldTransform().rotation) * owner->getWorldTransform().scale.x;
		start.y *= sin(owner->getWorldTransform().rotation) * owner->getWorldTransform().scale.y;


		// MEBBE RIGHT
		return Transform(start + owner->getWorldLocation(), trans.rotation + owner->getWorldRotation(), trans.scale);


	}
	
	return trans;

}

inline vec2 SceneComponent::getLocalLocation() const
{
	return trans.location;
}


inline vec2 SceneComponent::getScale() const
{
	return trans.scale;
}


inline float SceneComponent::getLocalRotation() const
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

