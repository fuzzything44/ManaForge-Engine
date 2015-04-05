
// redefinition of the one in TextureLibrary.h so the data is aligned correctly
struct UVData
{
	float2 bottomLeft;
	float2 topLeft;
	float2 bottomRight;
	float2 topRight;
};

// redefinition of the one in Actor.h so the data is aligned correctly -- order matters!
struct ActorData
{
	float2 location;
	float2 size;
	float2 velocity;
	float rotation;
	int renderOrder;
	struct UVData UVs;
	int loc; // pointer to the actor. TODO: Add 64-bit support with a long
	bool collides;
	// TODO: add collision channels -- need to make
};

// kernel used to figure out the collision. Takes in dynamic actor locations and sizes, and static actors locations and sizes.
__kernel void collide(
	__global struct ActorData* dynamicData,		// the Actor data for the dynamic (speed greater than one)
	__global struct ActorData* staticData,		// the Actor data for the static actors
	__global int* didCollide,					// set to one for collision and 0 for not
	__global void* outActorData,				// the out pointers to each actor that collided in pairs that collided 
	__private int dynamicSize,					// size of the dynamicData array
	__private int staticSize,					// size of the staticData array
	float deltaTime)				
{
	uint2 id = (uint2)(get_global_id(0), get_global_id(1));

	struct ActorData dataA = dynamicData[id.y];
	struct ActorData dataB = id.x > dynamicSize ? staticData[id.x - dynamicSize] : dynamicData[id.x];

	// Defining all the variables used for collision.

	// End time is the frame time (in milliseconds, should be pretty low.)
	float minTime = 0;
	float maxTime = deltaTime;

	float Vx1 = dataA.velocity.x;	// Velocities.
	float Vx2 = dataB.velocity.x;
	float Vy1 = dataA.velocity.y;
	float Vy2 = dataB.velocity.y;

	float xi1 = dataA.location.x;	// Initial locations.
	float xi2 = dataB.location.x;
	float yi1 = dataA.location.y;
	float yi2 = dataB.location.y;

	float L1 = dataA.size.x;	// Lengths and widths.
	float L2 = dataB.size.x;
	float W1 = dataA.size.y;
	float W2 = dataB.size.x;

	float r1 = dataA.rotation;	//Rotations.
	float r2 = dataB.rotation;

	if (r1 == 0 && r2 == 0) {	// If actors are axis-aligned.
		float recip;
		float minX;
		float maxX;
		float minY;
		float maxY;
	
		if (Vx1 != Vx2) {		//If velocities are not equal, do fancy equations.
			recip = 1/(Vx1 - Vx2);
			minX = (xi2 - xi1 - L1) * recip;
			maxX = (xi2 - xi1 + L2) * recip;
		} else {				// If velocities are equal, see if the x-values overlap.
			if (xi2 - L1 < xi1 && xi1 < xi2 + L2) {
				minX = 0;
				maxX = deltaTime;
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
				maxY = deltaTime;
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

	// Checking if it collides. Max time must be above min and min must be before end time.
	if ((maxTime > minTime) && (minTime < deltaTime) ) {
		didCollide[id.y] = 1;
	}

}



__kernel void update(
	__global float* outLoc,
	__global float2* outUV,
	__global uint* outElem,
	__global struct ActorData* data,
	__private float deltaTime)
{
	int id = get_global_id(0);


	data[id].location += deltaTime * data[id].velocity;
	
	struct ActorData localDat = data[id];
	
	// set rotation to be between 0 and 360
	for (localDat.rotation; localDat.rotation < 0; localDat.rotation += 360);
	for (localDat.rotation; localDat.rotation > 360; localDat.rotation -= 360);

	float3 finalLoc = (float3)(localDat.location.x, localDat.location.y, (-localDat.renderOrder));
	
	
	
	// the scale factor from degrees to radians
	// pi / 180
	float pi180 = M_PI / 180.f;

	///////////////////////////////////////////////////
	///////////////// LOCATION DATA ///////////////////
	///////////////////////////////////////////////////
	outLoc[id * 12] = finalLoc.x;
	outLoc[id * 12 + 1] = finalLoc.y;	// LOWER LEFT -- origin
	outLoc[id * 12 + 2] = finalLoc.z;

	////////////////// UPPER LEFT ////////////////////
	float2 upperLeft;

	// compute rotation
	if (localDat.rotation != 0.f)
	{
		float len = localDat.size.y;
		
		upperLeft.x = cos(localDat.rotation * pi180 + (M_PI / 2)) * len;
		upperLeft.y = sin(localDat.rotation * pi180 + (M_PI / 2)) * len;
	}

	// set upper left coordinates
	outLoc[id * 12 + 3] = finalLoc.x + upperLeft.x;
	outLoc[id * 12 + 4] = finalLoc.y + upperLeft.y;
	outLoc[id * 12 + 5] = finalLoc.z;


	////////////// LOWER RIGHT /////////////////////
	float2 lowerRight;

	// compute rotation
	if (localDat.rotation != 0.f)
	{
		float len = localDat.size.x;
		
		lowerRight.x = cos(localDat.rotation * pi180) * len;
		lowerRight.y = sin(localDat.rotation * pi180) * len;
	}

	// set upper left coordinates
	outLoc[id * 12 + 6] = finalLoc.x + lowerRight.x;
	outLoc[id * 12 + 7] = finalLoc.y + lowerRight.y;
	outLoc[id * 12 + 8] = finalLoc.z;

	//////////////// UPPER RIGHT ///////////////////
	float2 upperRight = (float2)(localDat.size.x, localDat.size.y);

	// compute rotation
	if (localDat.rotation != 0.f)
	{
		float len = sqrt(upperRight.x * upperRight.x + upperRight.y * upperRight.y);
		
		// convert to radians and add 45 deg
		upperRight.x = cos(localDat.rotation * pi180 + (M_PI / 4)) * len;
		upperRight.y = sin(localDat.rotation * pi180 + (M_PI / 4)) * len;
	}

	// set upper left coordinates
	outLoc[id * 12 + 9]  = finalLoc.x + upperRight.x;
	outLoc[id * 12 + 10] = finalLoc.y + upperRight.y;
	outLoc[id * 12 + 11] = finalLoc.z;

	//////////////////////////////////////////////
	////////////////// UV DATA ///////////////////
	//////////////////////////////////////////////
	outUV[id * 4]		= localDat.UVs.bottomLeft;

	outUV[id * 4 + 1]	= localDat.UVs.topLeft;

	outUV[id * 4 + 2]	= localDat.UVs.bottomRight;
	
	outUV[id * 4 + 3]	= localDat.UVs.topRight;


	//////////////////////////////////////////////
	////////////////// ELEM DATA /////////////////
	//////////////////////////////////////////////
	// first tri
	outElem[id * 6] = id * 4;
	outElem[id * 6 + 1] = id * 4 + 1;
	outElem[id * 6 + 2] = id * 4 + 2;

	// second tri
	outElem[id * 6 + 3] = id * 4 + 1;
	outElem[id * 6 + 4] = id * 4 + 2;
	outElem[id * 6 + 5] = id * 4 + 3;
	
}

__kernel void belloch_scan(
	__global int* in,
	__global int* out,
	int length)
{
	__private uint idx = get_global_id(0);

	
}