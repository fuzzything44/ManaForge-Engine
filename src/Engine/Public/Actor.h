#pragma once
#include "Engine.h"

#include "Transform.h"
#include "Logging.h"
#include "World.h"
#include "ActorTransformController.h"
#include "Runtime.h"
#include "SaveData.h"

#include <vector>

static const unsigned int MAX_ACTORS = 100001;

class Component;

/// <summary> An actor. </summary>
class Actor
{
	friend class boost::serialization::access;

public:
	
	// Make Chunk a freind so it can access private methods (namely tick)
	friend Component;

	// the global ID for this instatnce of the actor -- used mainly for networking
	actorLocation* GUID;
	
	/// <summary> Actors.</summary>
	///
	/// <param name="dataIn">  The data in.</param>
	/// <param name="chunkIn"> [in,out] If non-null, the chunk in.</param>
	///
	/// <returns> An ENGINE_API.</returns>
	ENGINE_API explicit Actor();

	//<summary> returns if the actor should be saved or not.
	ENGINE_API virtual bool needsSave();

	/// <summary> destructor </summary>
	ENGINE_API virtual ~Actor();


	//////////////////////// TRANSFORM MODIFICATION START ///////////////////
	inline Transform getWorldTransform() const;
	inline vec2 getWorldLocation() const;
	inline vec2 getScale() const;
	inline float getWorldRotation() const;

	inline void setScale(const vec2& newScale);
	inline void setWorldLocation(const vec2& newLoc);
	inline void setWorldRotation(float newRot);
	inline void setWorldTransform(const Transform& newTrans);

	inline void addWorldLocation(vec2 locToAdd);
	inline void addWorldRotation(float rotToAdd);
	//////////////////////// TRANSFORM MODIFICATION END /////////////////////

	inline mat3 getModelMatrix();

	inline PhysicsType getPhysicsType() const;
	inline void setPhysicsType(PhysicsType newType);

	inline vec2 getVelocity() const;
	inline void setVelocity(const vec2& newVelocity);

	inline void applyLocalForce(vec2 localForce, vec2 localPoint);
	inline void applyWorldForce(vec2 worldForce, vec2 worldPoint);

	inline void setAngularVelocity(float newVelocity);
	inline float getAngularVelocity();

	inline void applyTorque(float magnituede);

protected:

	ActorTransformController* transController;

	/// <summary> Ticks the given delta time.</summary>
	///
	/// <param name="deltaTime"> The delta time.</param>
	ENGINE_API virtual void tick(float deltaTime);

	std::vector<Component*> components;


	// save and load functions    
	template<class Archive>
	inline void save(Archive & ar, const unsigned int version) const;

	template<class Archive>
	inline void load(Archive & ar, const unsigned int version);

	BOOST_SERIALIZATION_SPLIT_MEMBER();

};


////////////////////////////////////////////////////////////////
///// INLINE DEFINITIONS ///////////////////////////////////////
////////////////////////////////////////////////////////////////


template<class Archive>
inline void Actor::save(Archive & ar, const unsigned int version) const
{
	Transform trans = transController->getTransform();
	vec2 velocity = transController->getVelocity();
	ar  & BOOST_SERIALIZATION_NVP(trans);
	ar  & BOOST_SERIALIZATION_NVP(velocity);
}
template<class Archive>
inline void Actor::load(Archive & ar, const unsigned int version)
{
	Transform trans;
	vec2 velocity;
	ar  & BOOST_SERIALIZATION_NVP(trans);
	ar  & BOOST_SERIALIZATION_NVP(velocity);

	transController->setTransform(trans);
	transController->setVelocity(velocity);
}

BOOST_CLASS_EXPORT_KEY2(Actor, "Default.Actor");

#include "Component.h"


/////////////////// START TRANSFORM MANIPULATION ///////////////////////////

inline Transform Actor::getWorldTransform() const
{ 
	return transController->getTransform();
}
inline vec2 Actor::getWorldLocation() const
{
	const Transform& trans = transController->getTransform();
	return trans.location;

}
inline vec2 Actor::getScale() const
{

	const Transform& trans = transController->getTransform();
	return trans.scale;
}
inline float Actor::getWorldRotation() const
{

	const Transform& trans = transController->getTransform();
	return trans.rotation;
}

inline void Actor::setScale(const vec2& newScale)
{
	Transform trans = transController->getTransform();
	trans.scale = newScale;
	transController->setTransform(trans);
}
inline void Actor::setWorldLocation(const vec2& newLoc)
{
	Transform trans = transController->getTransform();
	trans.location = newLoc;
	transController->setTransform(trans);
}
inline void Actor::setWorldRotation(float newRot)
{
	Transform trans = transController->getTransform();
	trans.rotation = newRot;
	transController->setTransform(trans);
}
inline void Actor::setWorldTransform(const Transform& newTrans)
{
	transController->setTransform(newTrans);
}

inline void Actor::addWorldLocation(vec2 locToAdd)
{
	Transform trans = transController->getTransform();
	trans.location += locToAdd;
	transController->setTransform(trans);
}
inline void Actor::addWorldRotation(float rotToAdd)
{
	Transform trans = transController->getTransform();
	trans.rotation += rotToAdd;
	transController->setTransform(trans);
}

/////////////////// END TRANSFORM MANIPULATION ///////////////////////////

inline mat3 Actor::getModelMatrix()
{
	mat3 ret;

	const Transform trans = transController->getTransform();

	ret = glm::translate(ret, trans.location);
	ret = glm::rotate(ret, trans.rotation);
	ret = glm::scale(ret, trans.scale);
	
	return ret;
}



inline PhysicsType Actor::getPhysicsType() const
{
	return transController->getType();
}
inline void Actor::setPhysicsType(PhysicsType newType)
{
	transController->setType(newType);
}


inline vec2 Actor::getVelocity() const
{
	return transController->getVelocity();
}
inline void Actor::setVelocity(const vec2& newVelocity)
{
	return transController->setVelocity(newVelocity);
}


inline void Actor::applyLocalForce(vec2 force, vec2 point)
{
	transController->applyLocalForce(force, point);
}

inline void Actor::applyWorldForce(vec2 worldForce, vec2 worldPoint)
{
	transController->applyWorldForce(worldForce, worldPoint);
}

inline void Actor::applyTorque(float magnitude)
{
	transController->applyTorque(magnitude);
}


inline void Actor::setAngularVelocity(float newVelocity)
{
	transController->setAngularVelocity(newVelocity);
}
inline float Actor::getAngularVelocity()
{
	return transController->getAngularVelocity();
}