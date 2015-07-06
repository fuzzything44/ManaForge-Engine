#pragma once

#include "OpenALAudioSystemConfig.h"

#include <AudioSystem.h>

#include <list>
#include <map>

class OpenALSoundSource;
class OpenALSoundCue;

class OpenALAudioSystem : public AudioSystem
{
public:
	OpenALAudioSystem();

	virtual std::shared_ptr<SoundCue> newSoundCue(const std::string& name) override;
	virtual std::unique_ptr<SoundSource> newSoundSource(SoundCue& cue, AudioComponent& owner) override;

	void addSource(OpenALSoundSource* source);

	bool update();

private:

	std::list<OpenALSoundSource*> sources;

	std::map<std::string, std::shared_ptr<OpenALSoundCue> > cues;

};