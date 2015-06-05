#include "OpenALSoundSource.h"

#include "OpenALSoundCue.h"

OpenALSoundSource::OpenALSoundSource(OpenALSoundCue* cue, AudioComponent* owner)
	:ownerComponent(owner)
{
	alGenSources(1, &sourceHandle);
	alSourcei(sourceHandle, AL_BUFFER, cue->bufferHandle);


}

void OpenALSoundSource::setLoops(bool loops)
{
	alSourcei(sourceHandle, AL_LOOPING, loops);
}

bool OpenALSoundSource::getLoops() const
{
	ALint loops;
	alGetSourcei(sourceHandle, AL_LOOPING, &loops);

	return loops != 0; // convert to boolean
}

void OpenALSoundSource::play()
{
	alSourcePlay(sourceHandle);
}

void OpenALSoundSource::pause()
{
	alSourcePause(sourceHandle);
}

void OpenALSoundSource::stop()
{
	alSourceStop(sourceHandle);
	alSourceRewind(sourceHandle);
}

AudioComponent* OpenALSoundSource::getOwnerComponent()
{
	return ownerComponent;
}


void OpenALSoundSource::update()
{
	vec2 loc = ownerComponent->getWorldLocation();

	alSource3f(sourceHandle, AL_POSITION, loc.x, loc.y, 0.f);

	// TODO: set velocity
}
