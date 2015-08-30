#include "OpenALSoundSource.h"

#include "OpenALSoundCue.h"
#include "OpenALAudioSystem.h"

#include <Runtime.h>
#include <Helper.h>

OpenALSoundSource::OpenALSoundSource(OpenALSoundCue& cue, AudioComponent& owner, OpenALAudioSystem& system)
	: ownerComponent(owner)
	, system(system)
{
	alGenSources(1, &sourceHandle);
	alSourcei(sourceHandle, AL_BUFFER, cue.bufferHandle);

	system.addSource(this);
}

void OpenALSoundSource::setLoops(bool loops) { alSourcei(sourceHandle, AL_LOOPING, loops); }

bool OpenALSoundSource::getLoops() const
{
	ALint loops;
	alGetSourcei(sourceHandle, AL_LOOPING, &loops);

	return loops != 0; // convert to boolean
}

void OpenALSoundSource::play() { alSourcePlay(sourceHandle); }

void OpenALSoundSource::pause() { alSourcePause(sourceHandle); }

void OpenALSoundSource::stop()
{
	alSourceStop(sourceHandle);
	alSourceRewind(sourceHandle);
}

AudioComponent& OpenALSoundSource::getOwnerComponent() { return ownerComponent; }

void OpenALSoundSource::setMaxDistance(float maxDistacne)
{
	alSourcef(sourceHandle, AL_MAX_DISTANCE, maxDistacne);
}

float OpenALSoundSource::getMaxDistance() const
{
	float ret;

	alGetSourcef(sourceHandle, AL_MAX_DISTANCE, &ret);

	return ret;
}

void OpenALSoundSource::setRolloffFactor(float rolloffFactor)
{
	alSourcef(sourceHandle, AL_ROLLOFF_FACTOR, rolloffFactor);
}

float OpenALSoundSource::getRolloffFactor() const
{
	float ret;
	alGetSourcef(sourceHandle, AL_ROLLOFF_FACTOR, &ret);
	return ret;
}

void OpenALSoundSource::setReferenceDistance(float refrenceDistance)
{
	alSourcef(sourceHandle, AL_REFERENCE_DISTANCE, refrenceDistance);
}

float OpenALSoundSource::getReferenceDistance() const
{
	float ret;
	alGetSourcef(sourceHandle, AL_REFERENCE_DISTANCE, &ret);
	return ret;
}

void OpenALSoundSource::update()
{
	auto&& loc		= ownerComponent.getWorldLocation();
	auto&& velocity = ownerComponent.getOwner().getVelocity();

	// update runtime variables
	alSource3f(sourceHandle, AL_POSITION, loc.x, loc.y, 0.f);
	alSource3f(sourceHandle, AL_VELOCITY, velocity.x, velocity.y, 0.f);
}

void OpenALSoundSource::setConeParams(vec2 direction, float innerAngle, float outerAngle, float outerGain)
{
	alSource3f(sourceHandle, AL_DIRECTION, direction.x, direction.y, 0.f);
	alSourcef(sourceHandle, AL_CONE_INNER_ANGLE, innerAngle);
	alSourcef(sourceHandle, AL_CONE_OUTER_ANGLE, outerAngle);
	alSourcef(sourceHandle, AL_CONE_OUTER_GAIN, outerGain);
}

void OpenALSoundSource::setGainBounds(float min, float max)
{
	alSourcef(sourceHandle, AL_MIN_GAIN, min);
	alSourcef(sourceHandle, AL_MAX_GAIN, max);
}
