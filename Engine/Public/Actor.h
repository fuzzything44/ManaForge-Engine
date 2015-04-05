// file:	public\Actor.h
//
// summary:	Declares the actor class
#pragma once
#include "Engine.h"
#include "TextureLibrary.h"

struct ActorData;
class Actor;
#include "CLHandler.h"

static const unsigned int MAX_ACTORS = 100001;

class Actor;
#include "Chunk.h"


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
		collides(collidesIn) { }

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
	static ENGINE_API GLint init(GLuint programIn, mat4* viewMatIn);

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	ENGINE_API vec2 getLocation();

	/// <summary> Gets the size.</summary>
	///
	/// <returns> The size.</returns>
	ENGINE_API vec2 getSize();

	/// <summary> Gets the rotation.</summary>
	///
	/// <returns> The rotation.</returns>
	ENGINE_API float getRotation();

	/// <summary> Gets the velocity.</summary>
	///
	/// <returns> The velocity.</returns>
	ENGINE_API vec2 getVelocity();

	/// <summary> Sets a location.</summary>
	///
	/// <param name="newLoc"> The new location.</param>
	ENGINE_API void setLocation(vec2 newLoc);

	/// <summary> Sets a size.</summary>
	///
	/// <param name="newSize"> Size of the new.</param>
	ENGINE_API void setSize(vec2 newSize);

	/// <summary> Sets a rotation.</summary>
	///
	/// <param name="newRot"> The new rot.</param>
	ENGINE_API void setRotation(float newRot);

	/// <summary> Sets a velocity.</summary>
	///
	/// <param name="newVelocity"> The new velocity.</param>
	ENGINE_API void setVelocity(vec2 newVelocity);

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
	template <typename T> static Actor* addActor(vec2 locationIn, vec2 sizeIn, vec2 velocityIn, float rotationIn, int renderOrderIn,
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

	virtual ~Actor();

protected:

	/// <summary> true to needs update.</summary>
	bool needsUpdate = false;

	/// <summary> The chunk.</summary>
	Chunk* chunk;

	/// <summary> Ticks the given delta time.</summary>
	///
	/// <param name="deltaTime"> The delta time.</param>
	virtual void tick(GLfloat deltaTime);

	/// <summary> Draw actors.</summary>
	///
	/// <param name="data">		    [in,out] The data.</param>
	/// <param name="deltaTime">    The delta time.</param>
	/// <param name="characterLoc"> [in,out] If non-null, the character location.</param>
	static void drawActors(std::vector<ActorData>& data, float deltaTime, Actor* characterLoc);

	/// <summary> Actors.</summary>
	///
	/// <param name="dataIn">  The data in.</param>
	/// <param name="chunkIn"> [in,out] If non-null, the chunk in.</param>
	///
	/// <returns> An ENGINE_API.</returns>
	ENGINE_API Actor(const ActorData& dataIn, Chunk* chunkIn);

	/// <summary> called when this actor overlaps with another actor </summary>
	/// <param ='otherActor'> the actor that it collides with </param>
	virtual void collide(Actor* otherActor) {	}

	/// <summary> The data.</summary>
	ActorData data;

	/// <summary> The buffer.</summary>
	cl::Buffer buff;
	
	/// <summary> The view matrix.</summary>
	static mat4* viewMat;

	/// <summary> The program.</summary>
	static GLuint program;

	/// <summary> Handle of the tex uniform.</summary>
	static GLint texUniformHandle;

	/// <summary> Handle of the view matrix uniform.</summary>
	static GLint viewMatUniformHandle;
	
	/// <summary> Handle of the character location uniform.</summary>
	static GLint characterLocUniformHandle;
	
	/// <summary> Array of vertices.</summary>
	static GLuint vertexArray;

	/// <summary> Buffer for location data.</summary>
	static GLuint locBuffer;

	/// <summary> Buffer for uv data.</summary>
	static GLuint UVBuffer;

	/// <summary> Buffer for element data.</summary>
	static GLuint elemBuffer;

};