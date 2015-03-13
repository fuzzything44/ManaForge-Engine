/**
 * \file	public\Actor.h
 *
 * \brief	Declares the actor class.
 */

#pragma once
#include "Engine.h"
#include "TextureLibrary.h"

struct ActorData;
class Actor;
#include "CLHandler.h"


// you will learn about this later -- ask me if you want to know
class Actor;
#include "Chunk.h"

/**
 * \struct	ActorData
 *
 * \brief	struct containing all of the Data for OpenCL.
 *
 * \author	Russell
 * \date	3/12/2015
 */

struct ActorData
{
	ActorData(vec2 locationIn = vec2(0.f, 0.f), vec2 sizeIn = vec2(0.f, 0.f), vec2 velocityIn = vec2(0.f, 0.f),
		float rotationIn = 0.f, int renderOrderIn = 0, UVData& UVsIn = UVData(), Actor* actorPtrIn = NULL, bool collidesIn = false)
		: location(locationIn),
		size(sizeIn),
		velocity(velocityIn),
		rotation(rotationIn),
		renderOrder(renderOrderIn),
		UVs(UVsIn),
		actorPtr(actorPtrIn),
		collides(collidesIn) { }

	/** \brief	The location. */
	vec2 location;
	/** \brief	The size. */
	vec2 size;
	/** \brief	The velocity. */
	vec2 velocity;
	/** \brief	The rotation. */
	float rotation;
	/** \brief	The render order. */
	int renderOrder;
	/** \brief	The vs. */
	UVData UVs;
	/** \brief	The actor pointer. */
	Actor* actorPtr;
	/** \brief	true to collides. */
	bool collides;
	// TODO: add collision channels -- need to make
};

/**
 * \class	Actor
 *
 * \brief	the actor class -- by default dynamic.
 *
 * \author	Russell
 * \date	3/12/2015
 */

class Actor
{
public:
	// Make Chunk a freind so it can access private methods (namely tick)
	friend Chunk;

	// let CLHandler access private varibles
	friend CLHandler;


	ENGINE_API GLint init();

	/**
	 * \fn	ENGINE_API vec2 Actor::getLocation();
	 *
	 * \brief	getters.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The location.
	 */

	ENGINE_API vec2 getLocation();

	/**
	 * \fn	ENGINE_API vec2 Actor::getSize();
	 *
	 * \brief	Gets the size.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The size.
	 */

	ENGINE_API vec2 getSize();

	/**
	 * \fn	ENGINE_API float Actor::getRotation();
	 *
	 * \brief	Gets the rotation.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The rotation.
	 */

	ENGINE_API float getRotation();

	/**
	 * \fn	ENGINE_API vec2 Actor::getVelocity();
	 *
	 * \brief	Gets the velocity.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The velocity.
	 */

	ENGINE_API vec2 getVelocity();

	/**
	 * \fn	ENGINE_API void Actor::setLocation(vec2 newLoc);
	 *
	 * \brief	setters.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	newLoc	The new location.
	 */

	ENGINE_API void setLocation(vec2 newLoc);

	/**
	 * \fn	ENGINE_API void Actor::setSize(vec2 newSize);
	 *
	 * \brief	Sets a size.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	newSize	Size of the new.
	 */

	ENGINE_API void setSize(vec2 newSize);

	/**
	 * \fn	ENGINE_API void Actor::setRotation(float newRot);
	 *
	 * \brief	Sets a rotation.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	newRot	The new rot.
	 */

	ENGINE_API void setRotation(float newRot);

	/**
	 * \fn	ENGINE_API void Actor::setVelocity(vec2 newVelocity);
	 *
	 * \brief	Sets a velocity.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	newVelocity	The new velocity.
	 */

	ENGINE_API void setVelocity(vec2 newVelocity);


	template <typename T>
	static Actor* addActor(vec2 locationIn, vec2 sizeIn, vec2 velocityIn, float rotationIn, int renderOrderIn,
		bool collides, UVData UVs, bool isPersisitent)
	{
		ActorData newDat(locationIn, sizeIn, velocityIn, rotationIn, renderOrderIn, UVs, NULL, collides);


		if (isPersisitent)
		{

			return new Actor(newDat, Chunk::persistentChunk);

		}
		else
		{
			return new Actor(newDat, Chunk::chunks[(GLuint)floorf(locationIn.x / (GLfloat)CHUNK_WIDTH)][(GLuint)floorf(locationIn.y / (GLfloat)CHUNK_WIDTH)]);
		}
	}

protected:

	/** \brief	true to needs update. */
	bool needsUpdate = false;

	/** \brief	refrence to the chunk that it resides in. */
	Chunk* chunk;

	/**
	 * \fn	virtual void Actor::tick(GLfloat deltaTime);
	 *
	 * \brief	called every frame if the variable ticks is true.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	deltaTime	the time in milliseconds since the previous time.
	 */

	virtual void tick(GLfloat deltaTime);

	/**
	 * \fn	ENGINE_API Actor::Actor(const ActorData& dataIn, Chunk* chunkIn);
	 *
	 * \brief	Actors.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	dataIn		   	The data in.
	 * \param [in,out]	chunkIn	If non-null, the chunk in.
	 *
	 * \return	An ENGINE_API.
	 */

	static void drawActors(int num);

	ENGINE_API Actor(const ActorData& dataIn, Chunk* chunkIn);

	/// <summary> called when this actor overlaps with another actor </summary>
	/// <param ='otherActor'> the actor that it collides with </param>
	virtual void collide(Actor* otherActor) {	}

	/** \brief	holds all of the data for the class. */
	ActorData data;

	/** \brief	buffer for the ActorData. */
	cl::Buffer buff;
	
};