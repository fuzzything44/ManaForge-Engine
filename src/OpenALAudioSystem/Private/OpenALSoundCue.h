#pragma once

#include "OpenALAudioSystemConfig.h"

#include <string>

#include <SoundCue.h>

#include <AL/alut.h>

class OpenALSoundCue : public SoundCue
{
	friend class OpenALSoundSource;

  public:
	explicit OpenALSoundCue(const path_t& name);
	virtual path_t getName() const override;

  private:
	ALuint bufferHandle;
	path_t name;
};