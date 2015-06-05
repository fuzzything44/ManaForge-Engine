#pragma once

#include "OpenALAudioSystemConfig.h"

#include <SoundSource.h>
#include <AudioComponent.h>

#include <AL/alut.h>

class OpenALSoundCue;
class OpenALAudioSystem;

class OpenALSoundSource : public SoundSource
{
public:
	explicit OpenALSoundSource(OpenALSoundCue* cue, AudioComponent* ownerComponent, OpenALAudioSystem* system);

	virtual void setLoops(bool loops) override;
	virtual bool getLoops() const override;

	virtual void play() override;
	virtual void pause() override;
	virtual void stop() override;


	virtual void setMaxDistance(float maxDistacne) override;
	virtual float getMaxDistance() const override;

	virtual void setRolloffFactor(float rolloffFactr) override;
	virtual float getRolloffFactor() const override;

	virtual void setReferenceDistance(float refrenceDistance) override;
	virtual float getReferenceDistance() const override;

	virtual AudioComponent* getOwnerComponent() override;


	void update();

private:
	AudioComponent* ownerComponent;
	ALuint sourceHandle;
	OpenALAudioSystem* system;
};