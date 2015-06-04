#pragma once

#include "OpenALAudioSystemConfig.h"

#include <string>

#include <SoundCue.h>

#include <AL/alut.h>

class OpenALSoundCue : public SoundCue
{
	friend class OpenALSoundSource;
public:

	explicit OpenALSoundCue(const std::string& name);
	virtual std::string getName() const override;

private:

	ALuint bufferHandle;
	std::string name;
};