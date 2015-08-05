#pragma once

#include "Engine.h"
#include "SceneComponent.h"

class SoundCue;
class SoundSource;

class AudioComponent : public SceneComponent
{
  public:
	ENGINE_API explicit AudioComponent(
		Actor& owner, SoundCue& cue, Transform trans = Transform(), bool loops = false);

	inline void setLoops(bool loops);
	inline bool getLoops() const;

	inline void play();
	inline void pause();
	inline void stop();

	inline void setMaxDistance(float maxDistacne);
	inline float getMaxDistance() const;

	inline void setRolloffFactor(float rolloffFactor);
	inline float getRolloffFactor() const;

	inline void setReferenceDistance(float refrenceDistance);
	inline float getReferenceDistance() const;

	inline void setConeParams(
		vec2 direction = vec2(), float innerAngle = 360.f, float outerAngle = 360.f, float outerGain = 0.f);

	inline void setGainBounds(float min, float max);

  protected:
	std::unique_ptr<SoundSource> source;
};

/////////////////////////////////////////////
/// INLINE DEFINITIONS
#include <SoundSource.h>

inline void AudioComponent::setLoops(bool loops) { source->setLoops(loops); }

inline bool AudioComponent::getLoops() const { return source->getLoops(); }

inline void AudioComponent::play() { source->play(); }

inline void AudioComponent::pause() { source->pause(); }

inline void AudioComponent::stop() { source->stop(); }

inline void AudioComponent::setMaxDistance(float maxDistacne) { source->setMaxDistance(maxDistacne); }

inline float AudioComponent::getMaxDistance() const { source->getMaxDistance(); }

inline void AudioComponent::setRolloffFactor(float rolloffFactor) { source->setRolloffFactor(rolloffFactor); }

inline float AudioComponent::getRolloffFactor() const { return source->getRolloffFactor(); }

inline void AudioComponent::setReferenceDistance(float refrenceDistance)
{
	source->setReferenceDistance(refrenceDistance);
}

inline float AudioComponent::getReferenceDistance() const { return source->getReferenceDistance(); }

inline void AudioComponent::setConeParams(vec2 direction, float innerAngle, float outerAngle, float outerGain)
{
	source->setConeParams(direction, innerAngle, outerAngle, outerGain);
}

inline void AudioComponent::setGainBounds(float min, float max) { source->setGainBounds(min, max); }
