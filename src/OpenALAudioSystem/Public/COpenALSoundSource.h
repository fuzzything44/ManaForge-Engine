#pragma once
#include "Engine.h"

#include <memory>

#include <AL/alut.h>
#include <../../OpenALAudioSystem/alut/include/AL/alut.h>

struct OpenALSoundCue;

class COpenALSoundSource
{
public:
	inline explicit COpenALSoundSource(const std::shared_ptr<OpenALSoundCue>& cue = { nullptr })
	{
		if (cue)
			init(cue);
	}

	void init(const std::shared_ptr<OpenALSoundCue>& cue_);

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

	// data
	std::shared_ptr<OpenALSoundCue> cue;
	ALuint sourceHandle;
};

#include "OpenALSoundCue.h"

void COpenALSoundSource::init(const std::shared_ptr<OpenALSoundCue>& cue_)
{
	cue = cue_;

	alGenSources(1, &sourceHandle);
	alSourcei(sourceHandle, AL_BUFFER, cue->bufferHandle);
}