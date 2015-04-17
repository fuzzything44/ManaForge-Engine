#include "Actor.h"
#include "CLHandler.h"
#include "ModuleManager.h"

///////////// STATIC VARIBALE INITALIZATION ////////////////////
Actor::Actor(const ActorData& dataIn, Chunk* chunkIn) 
	: chunk(chunkIn)
{STACK
	
	// add this class to the chunk
	chunk->actors.push_back(this);

}

void Actor::init(uint32 programIn, mat4* viewMatIn)
{STACK
	

}


void Actor::tick(float deltaTime)
{STACK
	
}



Actor::~Actor()
{STACK

}