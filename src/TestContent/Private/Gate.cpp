#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>

#define SAVE_DATA Gate, Actor, isOpen
#include REGISTER_FOR_SAVING_SOURCE()

Gate::Gate(const Transform& trans)
	: Actor(trans)
{

}

Gate::~Gate()
{

}
