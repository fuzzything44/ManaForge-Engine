#pragma once
#include "Engine.h"
#include "Transform.h"
#include "Logging.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <tuple>

static const unsigned int MAX_ACTORS = 100001;

class Chunk;
class Component;

/// <summary> An actor. </summary>
class Actor
{
	friend class boost::serialization::access;

public:

	// Make Chunk a freind so it can access private methods (namely tick)
	friend Chunk;
	friend Component;

	/// <summary> Actors.</summary>
	///
	/// <param name="dataIn">  The data in.</param>
	/// <param name="chunkIn"> [in,out] If non-null, the chunk in.</param>
	///
	/// <returns> An ENGINE_API.</returns>
	explicit ENGINE_API Actor(Transform trans = Transform{}, bool isPersistent = false);

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	inline vec2 getLocation(); 

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	inline vec2 getScale();

	/// <summary> Gets the rotation.</summary>
	///
	/// <returns> The rotation.</returns>
	inline float getRotation();

	/// <summary> Gets the velocity.</summary>
	///
	/// <returns> The velocity.</returns>
	inline vec2 getVelocity();

	/// <summary> Sets a location.</summary>
	///
	/// <param name="newLoc"> The new location.</param>
	inline void setLocation(vec2 newLoc);

	/// <summary> Sets a size.</summary>
	///
	/// <param name="newSize"> Size of the new.</param>
	inline void setScale(vec2 newScale);

	/// <summary> Sets a rotation.</summary>
	///
	/// <param name="newRot"> The new rot.</param>
	inline void setRotation(float newRot);

	/// <summary> Sets a velocity.</summary>
	///
	/// <param name="newVelocity"> The new velocity.</param>
	inline void setVelocity(vec2 newVelocity);
	
	virtual ~Actor();

protected:

	vec2 velocity;

	/// <summary> The chunk.</summary>
	Chunk* chunk;

	Transform trans;

	/// <summary> Ticks the given delta time.</summary>
	///
	/// <param name="deltaTime"> The delta time.</param>
	virtual void tick(float deltaTime);

	std::vector<Component*> components;

	// serialization function for boost
	template<typename Archive>
	void serialize(Archive& ar, uint32 version)
	{
		ar & BOOST_SERIALIZATION_NVP(trans);
		ar & BOOST_SERIALIZATION_NVP(velocity);
	}

};



////////////////////////////////////////////////////////////////
///// INLINE DEFINITIONS ///////////////////////////////////////
////////////////////////////////////////////////////////////////

#include "Chunk.h"
#include "Component.h"


inline vec2 Actor::getLocation()
{
	return trans.location;
}


inline vec2 Actor::getScale()
{
	return trans.scale;
}


inline float Actor::getRotation()
{
	return trans.rotation;
}


inline vec2 Actor::getVelocity()
{
	return velocity;
}


inline void Actor::setLocation(vec2 newLoc)
{
	trans.location = newLoc;
}


inline void Actor::setScale(vec2 newScale)
{
	trans.scale = newScale;
}


inline void Actor::setRotation(float newRot)
{
	trans.rotation= newRot;
}


inline void Actor::setVelocity(vec2 newVelocity)
{
	STACK
		ENG_LOG(Stack::GetTraceString());
	velocity = newVelocity;
}