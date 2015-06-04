#include "OpenALAudioSystem.h"

#include "OpenALSoundCue.h"
#include "OpenALSoundSource.h"

#include <ENGException.h>
#include <Runtime.h>

#include "AL/alut.h"

#include <boost/lexical_cast.hpp>

OpenALAudioSystem::OpenALAudioSystem()
{
	if (alutInit(0, nullptr) == AL_FALSE)	// catch errors
	{
		FATAL_ERR("Failed to initalize alut!");
	}
}

void OpenALAudioSystem::addSource(OpenALSoundSource* source)
{
	sources.push_back(source);
}

SoundCue* OpenALAudioSystem::newSoundCue(const std::string& name)
{
	return new OpenALSoundCue(name);
}

SoundSource* OpenALAudioSystem::newSoundSource(SoundCue* cue, AudioComponent* owner)
{
	OpenALSoundCue* ALcue = static_cast<OpenALSoundCue*>(cue);

	return new OpenALSoundSource(ALcue, owner);
}


bool OpenALAudioSystem::update()
{
	Transform cameraTrans = Runtime::get().moduleManager.getRenderer().getCurrentCamera().getWorldTransform();

	// setup listener properities
	alListener3f(AL_POSITION, cameraTrans.location.x, cameraTrans.location.y, 0.f);

	float orientation[] = 
	{
		0.f, 0.f, -1.f, // forward vector
		0.f, 1.f, 0.f	// up vector
	};

	alListenerfv(AL_ORIENTATION, orientation);

	// TODO: listener velocity


	for (auto& elem : sources)
	{
		elem->update();
	}

	return true;
}
