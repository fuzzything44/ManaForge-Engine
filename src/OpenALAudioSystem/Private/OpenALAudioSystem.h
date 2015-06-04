#pragma once

#include "OpenALAudioSystemConfig.h"

#include <AudioSystem.h>

#include <list>

class OpenALSoundSource;

class OpenALAudioSystem : public AudioSystem
{
public:
	OpenALAudioSystem();

	virtual SoundCue* newSoundCue(const std::string& name) override;
	virtual SoundSource* newSoundSource(SoundCue* cue, AudioComponent* owner) override;

	void addSource(OpenALSoundSource* source);

	bool update();

private:

	std::list<OpenALSoundSource*> sources;

};