#pragma once
#include "Engine.h"


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

	template <typename T>
	static ENGINE_API GLvoid addActor(const vec4& bounds, const vec2& velocity, const float& rotation,
		GLboolean collides, UVData UVs, GLboolean isPersisitent);

	// getters
	vec2  getLocation();
	vec2  getSize();
	float getRotation();
	vec2  getVelocity();

	// setters
	void setLocation(vec2 newLoc);
	void setSize(vec2 newSize);
	void setRotation(float newRot);
	void setVelocity(vec2 newVelocity);

protected:

	bool needsUpdate = false;

	// refrence to the chunk that it resides in
	Chunk* chunk;

	/// <summary> called every frame if the variable ticks is true </summary>
	/// <param name='deltaTime'> the time in milliseconds since the previous time. </param>
	virtual void tick(GLfloat deltaTime);

	Actor(const ActorData& dataIn, Chunk* chunkIn);

	/// <summary> called when this actor overlaps with another actor </summary>
	/// <param ='otherActor'> the actor that it collides with </param>
	virtual void collide(Actor* otherActor) {	}

	// holds all of the data for the class
	ActorData data;
	
};