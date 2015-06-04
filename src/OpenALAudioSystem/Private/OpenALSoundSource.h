#pragma once

#include "OpenALAudioSystemConfig.h"

#include <SoundSource.h>
#include <AudioComponent.h>

#include <AL/alut.h>

class OpenALSoundCue;

class OpenALSoundSource : public SoundSource
{
public:
	explicit OpenALSoundSource(OpenALSoundCue* cue, AudioComponent* ownerComponent);

	virtual void setLoops(bool loops) override;
	virtual bool getLoops() const override;

	virtual void play() override;
	virtual void pause() override;
	virtual void stop() override;

	virtual AudioComponent* getOwnerComponent() override;


	void update();

private:
	AudioComponent* ownerComponent;
	ALuint sourceHandle;
};