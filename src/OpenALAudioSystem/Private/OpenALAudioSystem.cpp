#include "OpenALAudioSystem.h"

#include "OpenALSoundCue.h"
#include "OpenALSoundSource.h"

#include <ENGException.h>
#include <Runtime.h>
#include <Helper.h>
#include <Renderer.h>

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

	return new OpenALSoundSource(ALcue, owner, this);
}


bool OpenALAudioSystem::update()
{
	vec2 cameraLoc = Runtime::get().moduleManager.getRenderer().getCurrentCamera().getWorldLocation();

	// setup listener properities
	alListener3f(AL_POSITION, cameraLoc.x, cameraLoc.y, 0.f);

	float orientation[] = 
	{
		0.f, 1.f, 0.f, // forward vector	Facing positive Y
		0.f, 0.f, 1.f	// up vector		Z-up
	};

	alListenerfv(AL_ORIENTATION, orientation);

	// TODO: listener velocity


	for (auto& elem : sources)
	{
		elem->update();
	}

	return true;
}
