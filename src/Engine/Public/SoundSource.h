#pragma once

#include "Engine.h"

class AudioComponent;

class SoundSource
{
public:

	SoundSource() { }

	virtual void setLoops(bool loops) = 0;
	virtual bool getLoops() const = 0;

	virtual void play() = 0; 
	virtual void pause() = 0;
	virtual void stop() = 0;

	virtual AudioComponent* getOwnerComponent() = 0;

};