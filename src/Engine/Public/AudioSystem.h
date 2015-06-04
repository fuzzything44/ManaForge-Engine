#pragma once

#include "Engine.h"

#include <string>

#include <boost/noncopyable.hpp>

class SoundCue;
class SoundSource;
class AudioComponent;

class AudioSystem : boost::noncopyable
{
public:
	AudioSystem(){}

	virtual SoundCue* newSoundCue(const std::string& name) = 0;
	virtual SoundSource* newSoundSource(SoundCue* cue, AudioComponent* owner) = 0;
};