#pragma once

#include "Engine.h"
#include "SceneComponent.h"

class SoundCue;
class SoundSource;

class AudioComponent : public SceneComponent
{
public:
	ENGINE_API explicit AudioComponent(Actor* owner = nullptr, Transform trans = Transform{}, SoundCue* cue = nullptr, bool loops = false);

	inline void setLoops(bool loops);
	inline bool getLoops() const;
	 
	inline void play();
	inline void pause();
	inline void stop();

protected:
	SoundSource* source;
};

/////////////////////////////////////////////
/// INLINE DEFINITIONS
#include <SoundSource.h>

inline void AudioComponent::setLoops(bool loops)
{
	source->setLoops(loops);
}

inline bool AudioComponent::getLoops() const
{
	return source->getLoops();
}

inline void AudioComponent::play()
{
	source->play();
}

inline void AudioComponent::pause()
{
	source->pause();
}

inline void AudioComponent::stop()
{
	source->stop();
}
