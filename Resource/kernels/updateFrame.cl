// redefinition of the one in TextureLibrary.h so the data is aligned correctly
struct UVData
{
	float2 bottomLeft;
	float2 topLeft;
	float2 bottomRight;
	float2 topRight;
};

// redefination of the one in Actor.h so the data is aligned correctly -- order matters!
struct ActorData
{
	float4 bounds;
	float2 velocity;
	struct UVData UVs;
	void* loc; // pointer to the actor
	bool collides;
	// TODO: add collision channels -- need to make
};

// kernel used to figure out the collision. Takes in dynamic actor locations and sizes, and static actors locations and sizes.
__kernel void collide(
	__global struct ActorData* dynamicData,		// the Actor data for the dynamic (speed greater than one)
	__global struct ActorData* staticData,		// the Actor data for the static actors
	__global bool* didCollide,					// set to one for collision and 0 for not
	__global void* outActorData)				// the out pointers to each actor that collided in pairs that collided 
{
	// TODO: add actual collsion checking

	uint2 id = (uint2)(get_global_id(0), get_global_id(1));

	didCollide[id.y] = 0;

}
