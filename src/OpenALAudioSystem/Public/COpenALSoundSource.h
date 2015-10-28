#pragma once

#include "OpenALAudioSystemConfig.h"

#include <AL/alut.h>

struct OpenALSoundCue;
class OpenALAudioSystem;



class COpenALSoundSource
{
public:
	inline explicit COpenALSoundSource(std::shared_ptr<OpenALSoundCue>& cue);

	void setLoops(bool loops)
	{
		alSourcei(sourceHandle, AL_LOOPING, loops);
	}
	bool getLoops() const
	{
		ALint loops;
		alGetSourcei(sourceHandle, AL_LOOPING, &loops);

		return loops != 0; // convert to boolean
	}

	void play()
	{
		alSourcePlay(sourceHandle);
	}
	void pause()
	{
		alSourcePause(sourceHandle);
	}
	void stop()
	{
		alSourceStop(sourceHandle);
		alSourceRewind(sourceHandle);
	}

	void setMaxDistance(float maxDistacne)
	{
		alSourcef(sourceHandle, AL_MAX_DISTANCE, maxDistacne);
	}
	float getMaxDistance() const
	{
		float ret;

		alGetSourcef(sourceHandle, AL_MAX_DISTANCE, &ret);

		return ret;
	}

	void setRolloffFactor(float rolloffFactor)
	{
		alSourcef(sourceHandle, AL_ROLLOFF_FACTOR, rolloffFactor);
	}
	float getRolloffFactor() const
	{
		float ret;
		alGetSourcef(sourceHandle, AL_ROLLOFF_FACTOR, &ret);
		return ret;
	}

	void setReferenceDistance(float refrenceDistance)
	{
		alSourcef(sourceHandle, AL_REFERENCE_DISTANCE, refrenceDistance);
	}
	float getReferenceDistance() const
	{
		float ret;
		alGetSourcef(sourceHandle, AL_REFERENCE_DISTANCE, &ret);
		return ret;
	}

	void setConeParams(vec2 direction, float innerAngle, float outerAngle, float outerGain)
	{
		alSource3f(sourceHandle, AL_DIRECTION, direction.x, direction.y, 0.f);
		alSourcef(sourceHandle, AL_CONE_INNER_ANGLE, innerAngle);
		alSourcef(sourceHandle, AL_CONE_OUTER_ANGLE, outerAngle);
		alSourcef(sourceHandle, AL_CONE_OUTER_GAIN, outerGain);
	}

	void setGainBounds(float min, float max)
	{
		alSourcef(sourceHandle, AL_MIN_GAIN, min);
		alSourcef(sourceHandle, AL_MAX_GAIN, max);
	}

private:
	std::shared_ptr<OpenALSoundCue> cue;
	ALuint sourceHandle;
};

#include "OpenALSoundCue.h"

COpenALSoundSource::COpenALSoundSource(std::shared_ptr<OpenALSoundCue>& cue)
	:cue(cue)
{
	alGenSources(1, &sourceHandle);
	alSourcei(sourceHandle, AL_BUFFER, cue->bufferHandle);	
}
