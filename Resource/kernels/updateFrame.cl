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
	__global int* didCollide,					// set to one for collision and 0 for not
	__global void* outActorData)				// the out pointers to each actor that collided in pairs that collided 
{
	
	uint2 id = (uint2)(get_global_id(0), get_global_id(1));

	ActorData dataA = dynamicData[id.y];

	// Defining all the random variables used for collision.

	float endTime; 
	// End time is the frame time (in milliseconds, should be pretty low.)

	float minTime = 0;
	float maxTime = endTime;

	float Vx1;	// Velocities.
	float Vx2;
	float Vy1;
	float Vy2;

	float xi1;	// Initial locations.
	float xi2;
	float xi3;
	float xi4;

	float L1;	// Lengths and widths.
	float L2;
	float W1;
	float W2;

	float r1;	//Rotations.
	float r2;

	if (r1 == 0 && r2 == 0) {	// If actors are axis-aligned.
		float recip;
		float minX;
		float maxX;
		float minY;
		float maxY;
	
		if (Vx1 != Vx2) {		//If velocities are not equal, do fancy equations.
			recip = 1/(Vx1 - vx2);
			minX = (xi2 - xi1 - L1) * recip;
			maxX = (xi2 - xi1 + L2) * recip;
		} else {				// If velocities are equal, see if the x-values overlap.
			if (xi2 - L1 < xi1 && xi1 < xi2 + L2) {
				minX = 0;
				maxX = endTime;
			} else {
				minX = 0;
				maxX = 0;
			}
		}
		// Do the same, but with Y
		if (Vy1 != Vy2) {
			recip = 1/(Vy1 - Vy2);
			minY = (yi2 - yi1 - W1) * recip;
			maxY = (yi2 - yi1 + W2) * recip;
		} else {
			if (yi2 - W1 < yi1 && yi1 < yi2 + W2) {
				minY = 0;
				maxY = endTime;
			} else {
				minY = 0;
				maxY = 0;
			}
		}

		float minTime = minX < minY ? minY : minX;
		float maxTime = maxX > maxY ? maxY : maxX;
	} else {
		// Non axis-aligned collision here
	
	}

	didCollide[id.y] = 0;

	// Checking if it collides. Max time must be above min and min must be before end time.
	if ((maxTime > minTime) && (minTime < endTime) ) {
		didCollide[id.y] = 1;
	}

}

__kernel void belloch_scan(
	__global int* in,
	__global int* out,
	int length)
{
	__private uint idx = get_global_id(0);

	
}