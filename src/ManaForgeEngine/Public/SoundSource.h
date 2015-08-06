#pragma once

#include "Engine.h"

class AudioComponent;

class SoundSource
{
public:
	SoundSource() = default;

	virtual void play() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;

	virtual void setMaxDistance(float maxDistacne) = 0;
	virtual float getMaxDistance() const = 0;

	virtual void setLoops(bool loops) = 0;
	virtual bool getLoops() const = 0;

	virtual void setRolloffFactor(float rolloffFactor) = 0;
	virtual float getRolloffFactor() const = 0;

	virtual void setReferenceDistance(float refrenceDistance) = 0;
	virtual float getReferenceDistance() const = 0;

	virtual void setConeParams(vec2 direction, float innerAngle, float outerAngle, float outerGain) = 0;

	virtual void setGainBounds(float min, float max) = 0;

	virtual AudioComponent& getOwnerComponent() = 0;
};