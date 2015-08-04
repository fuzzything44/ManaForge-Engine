#include "EnginePCH.h"

#include "AudioComponent.h"

#include "SoundCue.h"
#include "SoundSource.h"
#include "Runtime.h"
#include "AudioSystem.h"

AudioComponent::AudioComponent(Actor& owner, SoundCue& cue, Transform trans, bool loops)
    : SceneComponent(owner, trans), source(Runtime::get().audioSystem->newSoundSource(cue, *this))
{
	source->setLoops(loops);
}
