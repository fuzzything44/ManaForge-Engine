#pragma once
#include "Engine.h"
#include "UVData.h"

static const unsigned int MAX_ACTORS = 100001;


class Renderer;
class Model;
class CLHandler;
class Chunk;
struct ActorData;

/// <summary> An actor. </summary>
class Actor
{
public:
	// Make Chunk a freind so it can access private methods (namely tick)
	friend Chunk;

	// let CLHandler access private varibles
	friend CLHandler;

	/// <summary> Initialises this object.</summary>
	///
	/// <param name="programIn"> The program in.</param>
	/// <param name="viewMatIn"> [in,out] If non-null, the view matrix in.</param>
	///
	/// <returns>  the error code</returns>
	static ENGINE_API void init(uint32 programIn, mat4* viewMatIn);



	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	inline vec2 getLocation(); 

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	inline vec2 getSize();

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
	inline void setSize(vec2 newSize);

	/// <summary> Sets a rotation.</summary>
	///
	/// <param name="newRot"> The new rot.</param>
	inline void setRotation(float newRot);

	/// <summary> Sets a velocity.</summary>
	///
	/// <param name="newVelocity"> The new velocity.</param>
	inline void setVelocity(vec2 newVelocity);

	/// <summary> Adds an actor.</summary>
	///
	/// <typeparam name="T"> actor type to spawn.</typeparam>
	/// <param name="locationIn">    The location in.</param>
	/// <param name="sizeIn">		 The size in.</param>
	/// <param name="velocityIn">    The velocity in.</param>
	/// <param name="rotationIn">    The rotation in.</param>
	/// <param name="renderOrderIn"> The render order in.</param>
	/// <param name="collides">		 true to collides.</param>
	/// <param name="UVs">			 The vs.</param>
	/// <param name="isPersisitent"> true if this object is persisitent.</param>
	template <typename T> static Actor* addActor(
		vec2 locationIn, 
		vec2 sizeIn, 
		vec2 velocityIn, 
		float rotationIn, 
		uint8 renderOrderIn,
		bool collides, 
		UVData UVs, 
		bool isPersisitent);

	virtual ~Actor();

protected:

	vec2 velocity;

	/// <summary> The chunk.</summary>
	Chunk* chunk;

	/// <summary> The Model </summary>
	Model* model;

	/// <summary> Ticks the given delta time.</summary>
	///
	/// <param name="deltaTime"> The delta time.</param>
	virtual void tick(float deltaTime);

	/// <summary> Actors.</summary>
	///
	/// <param name="dataIn">  The data in.</param>
	/// <param name="chunkIn"> [in,out] If non-null, the chunk in.</param>
	///
	/// <returns> An ENGINE_API.</returns>
	ENGINE_API Actor(const ActorData& dataIn, Chunk* chunkIn);

	/// <summary> called when this actor overlaps with another actor </summary>
	/// <param ='otherActor'> the actor that it collides with </param>
	virtual void collide(Actor* /*otherActor*/) {	}
};


/// <summary> struct containing all of the Data for OpenCL </summary>
struct ActorData
{
	/// <summary> Constructor.</summary>
	///
	/// <param name="locationIn">    The location in.</param>
	/// <param name="sizeIn">		 The size in.</param>
	/// <param name="velocityIn">    The velocity in.</param>
	/// <param name="rotationIn">    The rotation in.</param>
	/// <param name="renderOrderIn"> The render order in.</param>
	/// <param name="UVsIn">		 [in,out] (Optional) the vs in.</param>
	/// <param name="actorPtrIn">    [in,out] (Optional) If non-null, the actor pointer in.</param>
	/// <param name="collidesIn">    true to collides in.</param>
	explicit ActorData(vec2 locationIn = vec2(0.f, 0.f), vec2 sizeIn = vec2(0.f, 0.f), vec2 velocityIn = vec2(0.f, 0.f),
		float rotationIn = 0.f, int renderOrderIn = 0, UVData UVsIn = UVData(), Actor* actorPtrIn = nullptr, bool collidesIn = false)
		: location(locationIn),
		size(sizeIn),
		velocity(velocityIn),
		rotation(rotationIn),
		renderOrder(renderOrderIn),
		UVs(UVsIn),
		actorPtr(actorPtrIn),
		collides(collidesIn) {
		STACK
	}


	/// <summary> The location of the actor </summary>
	vec2 location;

	/// <summary> The size.</summary>
	vec2 size;

	/// <summary> The velocity.</summary>
	vec2 velocity;

	/// <summary> The rotation.</summary>
	float rotation;

	/// <summary> The render order.</summary>
	int renderOrder;

	/// <summary> The vs.</summary>
	UVData UVs;

	/// <summary> The actor pointer.</summary>
	Actor* actorPtr;

	/// <summary> true to collides.</summary>
	bool collides;


	// TODO: add collision channels -- need to make
};


////////////////////////////////////////////////////////////////
///// INLINE DEFINITIONS ///////////////////////////////////////
////////////////////////////////////////////////////////////////

#include "Chunk.h"
#include "Renderer.h"

template <typename T> Actor* Actor::addActor(vec2 locationIn, vec2 sizeIn, vec2 velocityIn, float rotationIn, uint8 renderOrderIn,
	bool collides, UVData UVs, bool isPersisitent)
{
	STACK
		ActorData newDat(locationIn, sizeIn, velocityIn, rotationIn, renderOrderIn, UVs, nullptr, collides);


	if (isPersisitent)
	{

		return new Actor(newDat, Chunk::persistentChunk);
	}
	ivec2 id = uvec2(static_cast<uint16>(floorf(locationIn.x / static_cast<float>(CHUNK_WIDTH))),
		static_cast<uint32>(floorf(locationIn.y / static_cast<float>(CHUNK_WIDTH))));

	// check if the chunk exists
	if (0 <= id.x && id.x < static_cast<int16>(Chunk::getChunkSize().x), 0 <= id.y && id.y < static_cast<int16>(Chunk::getChunkSize().y))
	{


		return new Actor(newDat, Chunk::chunks[id.x][id.y]);
	}
	else
	{
		ENG_LOG("\nChunk couldn't be located at location defined. Adding to persistent chunk.");
		return new Actor(newDat, Chunk::persistentChunk);
	}
}


inline vec2 Actor::getLocation()
{
	return model->getLocation();
}


inline vec2 Actor::getSize()
{
	return model->getScale();
}


inline float Actor::getRotation()
{
	return model->getRotation();
}


inline vec2 Actor::getVelocity()
{
	return velocity;
}


inline void Actor::setLocation(vec2 newLoc)
{
	model->setLocation(newLoc);
}


inline void Actor::setSize(vec2 newSize)
{
	model->setScale(newSize);
}


inline void Actor::setRotation(float newRot)
{
	model->setRotation(newRot);
}


inline void Actor::setVelocity(vec2 newVelocity)
{
	velocity = newVelocity;
}