#pragma once
#include "Engine.h"
#include "TextureLibrary.h"

struct ActorData;
class Actor;
#include "CLHandler.h"


// you will learn about this later -- ask me if you want to know
class Actor;
#include "Chunk.h"

// struct containing all of the Data for OpenCL
struct ActorData
{
	ActorData(vec2 locationIn = vec2(0.f, 0.f), vec2 sizeIn = vec2(0.f, 0.f), vec2 velocityIn = vec2(0.f, 0.f), 
		float rotationIn = 0.f, UVData& UVsIn = UVData(), Actor* actorPtrIn = NULL, bool collidesIn = false) 
		: location(locationIn),
		size(sizeIn),
		velocity(velocityIn),
		rotation(rotationIn),
		UVs(UVsIn),
		actorPtr(actorPtrIn),
		collides(collidesIn) { }

	vec2 location;
	vec2 size;
	vec2 velocity;
	float rotation;
	int renderOrder;
	UVData UVs;
	Actor* actorPtr;
	bool collides;
	// TODO: add collision channels -- need to make
};


// the actor class -- by default dynamic 
class Actor
{
public:
	// Make Chunk a freind so it can access private methods (namely tick)
	friend Chunk;

	// let CLHandler access private varibles
	friend CLHandler;



	// getters
	ENGINE_API vec2 getLocation();
	ENGINE_API vec2 getSize();
	ENGINE_API float getRotation();
	ENGINE_API vec2 getVelocity();

	// setters
	ENGINE_API void setLocation(vec2 newLoc);
	ENGINE_API void setSize(vec2 newSize);
	ENGINE_API void setRotation(float newRot);
	ENGINE_API void setVelocity(vec2 newVelocity);


	template <typename T>
	static Actor* addActor(vec2 locationIn, vec2 sizeIn, vec2 velocityIn, float rotationIn,
		bool collides, UVData UVs, bool isPersisitent)
	{
		ActorData newDat(locationIn, sizeIn, velocityIn, rotationIn, UVs, NULL, collides);


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

	bool needsUpdate = false;

	// refrence to the chunk that it resides in
	Chunk* chunk;

	/// <summary> called every frame if the variable ticks is true </summary>
	/// <param name='deltaTime'> the time in milliseconds since the previous time. </param>
	virtual void tick(GLfloat deltaTime);

	
	ENGINE_API Actor(const ActorData& dataIn, Chunk* chunkIn);

	/// <summary> called when this actor overlaps with another actor </summary>
	/// <param ='otherActor'> the actor that it collides with </param>
	virtual void collide(Actor* otherActor) {	}

	// holds all of the data for the class
	ActorData data;

	// buffer for the ActorData
	cl::Buffer buff;
	
};