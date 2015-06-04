#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>
#include <AudioSystem.h>

#define SAVE_DATA Gate, Actor, isOpen
#include REGISTER_FOR_SAVING_SOURCE()

Gate::Gate(const Transform& trans)
	: Actor(trans)
{
	SoundCue* cue = Runtime::get().moduleManager.getAudioSystem().newSoundCue("test");
	audioComp = new AudioComponent(this, Transform{}, cue);
}

Gate::~Gate()
{

}
