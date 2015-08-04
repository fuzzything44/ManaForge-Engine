#pragma once

#include "Engine.h"

#include <string>
#include <memory>

#include <boost/noncopyable.hpp>

class SoundCue;
class SoundSource;
class AudioComponent;

class AudioSystem : boost::noncopyable
{
  public:
	virtual ~AudioSystem() = default;

	virtual std::shared_ptr<SoundCue> newSoundCue(const path_t& name) = 0;
	virtual std::unique_ptr<SoundSource> newSoundSource(SoundCue& cue, AudioComponent& owner) = 0;
};