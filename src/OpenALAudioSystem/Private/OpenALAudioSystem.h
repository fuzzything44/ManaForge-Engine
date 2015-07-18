#pragma once

#include "OpenALAudioSystemConfig.h"

#include <AudioSystem.h>

#include <list>
#include <unordered_map>

class OpenALSoundSource;
class OpenALSoundCue;

class OpenALAudioSystem : public AudioSystem
{
public:
	OpenALAudioSystem();

	virtual ~OpenALAudioSystem();

	virtual std::shared_ptr<SoundCue> newSoundCue(const path_t& name) override;
	virtual std::unique_ptr<SoundSource> newSoundSource(SoundCue& cue, AudioComponent& owner) override;

	void addSource(OpenALSoundSource* source);

	bool update();

private:

	std::list<OpenALSoundSource*> sources;

	std::unordered_map<path_t, std::shared_ptr<OpenALSoundCue> > cues;

};