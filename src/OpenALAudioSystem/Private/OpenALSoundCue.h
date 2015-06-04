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
private:
	ALuint bufferHandle;
};