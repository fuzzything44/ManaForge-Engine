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
	explicit SceneComponent(Actor& owner, Transform trans = Transform{});

	virtual ~SceneComponent() override;

	ENGINE_API inline Transform getRelativeTransform() const;

	ENGINE_API inline Transform getWorldTransform() const;

	ENGINE_API inline vec2 getWorldLocation() const;

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	ENGINE_API inline vec2 getRelativeLocation() const;

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	ENGINE_API inline vec2 getScale() const;

	/// <summary> Gets the rotation in radians.</summary>
	///
	/// <returns> The rotation.</returns>
	ENGINE_API inline float getRelativeRotation() const;

	/// <summary> Gets the rotation in radians.</summary>
	///
	/// <returns> The rotation.</returns>
	ENGINE_API inline float getWorldRotation() const;

	/// <summary> Sets a location.</summary>
	///
	/// <param name="newLoc"> The new location.</param>
	ENGINE_API inline void setRelativeLocation(vec2 newLoc);

	/// <summary> Sets a size.</summary>
	///
	/// <param name="newSize"> Size of the new.</param>
	ENGINE_API inline void setScale(vec2 newScale);

	/// <summary> Sets a rotation.</summary>
	///
	/// <param name="newRot"> The new rot.</param>
	ENGINE_API inline void setRelativeRotation(float newRot);

	ENGINE_API inline void setRelativeTransform(const Transform& newTrans);

	ENGINE_API inline void addRelativeLocation(vec2 locToAdd);

	/// <summray> Add a rotation (in radians) to the current rotation </summary>
	/// <param name="rotToAdd"> the roatation to add in radiands </param>
	ENGINE_API inline void addRelativeRotation(float rotToAdd);

	ENGINE_API inline mat3 getModelMatrix() const;

protected:
	Transform trans;
};

///////////////////////
///// INLINE DEFINITIONS
///////////////////////
#include "Actor.h"

inline SceneComponent::SceneComponent(Actor& owner, Transform trans)
	: Component(owner)
	, trans(trans)
{
}

inline SceneComponent::~SceneComponent() {}

inline Transform SceneComponent::getRelativeTransform() const { return trans; }

inline Transform SceneComponent::getWorldTransform() const
{

	mat3 model = getModelMatrix();

	Transform worldTrans;

	vec3 loc3 = model * vec3(0.f, 0.f, 1.f);
	worldTrans.location = vec2(loc3.x, loc3.y);
	worldTrans.rotation = owner.getWorldRotation() + trans.rotation;
	worldTrans.scale = owner.getScale() + trans.scale;

	return worldTrans;
}

inline vec2 SceneComponent::getWorldLocation() const
{
	mat3 model = getModelMatrix();

	vec3 loc3 = model * vec3(0.f, 0.f, 1.f);
	return vec2(loc3.x, loc3.y);
}

inline float SceneComponent::getWorldRotation() const { return owner.getWorldRotation() + trans.rotation; }

inline vec2 SceneComponent::getRelativeLocation() const { return trans.location; }

inline vec2 SceneComponent::getScale() const { return trans.scale; }

inline float SceneComponent::getRelativeRotation() const { return trans.rotation; }

inline void SceneComponent::setRelativeLocation(vec2 newLoc) { trans.location = newLoc; }

inline void SceneComponent::setScale(vec2 newScale) { trans.scale = newScale; }

inline void SceneComponent::setRelativeRotation(float newRot) { trans.rotation = newRot; }

inline void SceneComponent::setRelativeTransform(const Transform& newTrans) { trans = newTrans; }

inline void SceneComponent::addRelativeLocation(vec2 locToAdd) { trans.location += locToAdd; }

inline void SceneComponent::addRelativeRotation(float rotToAdd) { trans.rotation += rotToAdd; }

inline mat3 SceneComponent::getModelMatrix() const
{
	mat3 ret = owner.getModelMatrix();

	ret = glm::translate(ret, trans.location);
	ret = glm::rotate(ret, trans.rotation);
	ret = glm::scale(
		ret, trans.scale); // I REALLY DON'T GET THIS ORDER -- TO ME IT SHOULD BE THE OTHER WAY AROUND BUT IT
						   // WORKS

	return ret;
}