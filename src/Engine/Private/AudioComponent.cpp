#include "AudioComponent.h"

#include "SoundCue.h"
#include "SoundSource.h"
#include "Runtime.h" 
#include "AudioSystem.h"

AudioComponent::AudioComponent(Actor* owner, Transform trans, SoundCue* cue, bool loops)
	:SceneComponent(owner, trans),
	source(Runtime::get().moduleManager.getAudioSystem().newSoundSource(cue, this))
{
	source->setLoops(loops);
}

