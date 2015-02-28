#pragma once
#include "Engine.h"


// you will learn about this later -- ask me if you want to know
class Actor;
#include "Chunk.h"

// struct containing all of the Data for OpenCL
struct ActorData
{
	ActorData(vec4 boundsIn = vec4(0.f, 0.f, 0.f, 0.f), vec2 velocityIn = vec2(0.f, 0.f), 
		float rotationIn = 0.f, UVData UVsIn = UVData(), Actor* locIn = NULL, bool collidesIn = false) 
		: bounds(boundsIn),
		velocity(velocityIn),
		rotation(rotationIn),
		UVs(UVsIn),
		loc(locIn),
		collides(collidesIn) { }

	vec4 bounds;
	vec2 velocity;
	float rotation;
	UVData UVs;
	Actor* loc;
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
	static ENGINE_API GLvoid addActor(const vec4& boundsIn, GLfloat* UVs, GLboolean isPersisitent);

protected:

	// refrence to the chunk that it resides in
	Chunk* chunk;

	/// <summary> called every frame if the variable ticks is true </summary>
	/// <param name='deltaTime'> the time in milliseconds since the previous time. </param>
	virtual void tick(GLfloat deltaTime) {	}

	Actor(const ActorData& dataIn, Chunk* chunkIn);

	/// <summary> called when this actor overlaps with another actor </summary>
	/// <param ='otherActor'> the actor that it collides with </param>
	virtual void collide(Actor* otherActor) {	}

	// holds all of the data for the class
	ActorData data;
	
};