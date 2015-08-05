#include "OpenALAudioSystem.h"

#include "OpenALSoundCue.h"
#include "OpenALSoundSource.h"

#include <ENGException.h>
#include <Runtime.h>
#include <Helper.h>
#include <Renderer.h>
#include <CameraComponent.h>

#include "AL/alut.h"

#include <boost/lexical_cast.hpp>

OpenALAudioSystem::OpenALAudioSystem()
{
	if (alutInit(nullptr, nullptr) == AL_FALSE) // catch errors
	{
		ALenum err = alutGetError();
		MFLOG(Warning) << "Failed to initalize alut with error: " << alutGetErrorString(err)
					   << " And error number: " << err
					   << "\n If you recieve this error, you won't have sound.";
	}
}

OpenALAudioSystem::~OpenALAudioSystem() {}

void OpenALAudioSystem::addSource(OpenALSoundSource* source) { sources.push_back(source); }

std::shared_ptr<SoundCue> OpenALAudioSystem::newSoundCue(const path_t& name)
{
	auto iter = cues.find(name);

	if (iter != cues.end()) {
		return iter->second;
	}

	// return a new shared ptr from it
	return cues.insert({name, std::make_shared<OpenALSoundCue>(name)}).first->second;
}

std::unique_ptr<SoundSource> OpenALAudioSystem::newSoundSource(SoundCue& cue, AudioComponent& owner)
{
	return std::make_unique<OpenALSoundSource>(static_cast<OpenALSoundCue&>(cue), owner, *this);
}

bool OpenALAudioSystem::update()
{
	vec2 cameraLoc = Runtime::get().renderer->getCurrentCamera().getWorldLocation();

	// setup listener properities
	alListener3f(AL_POSITION, cameraLoc.x, cameraLoc.y, 0.f);

	float orientation[] = {
		0.f,
		1.f,
		0.f, // forward vector	Facing positive Y
		0.f,
		0.f,
		1.f // up vector		Z-up
	};

	alListenerfv(AL_ORIENTATION, orientation);

	// TODO: listener velocity

	for (auto& elem : sources)
	{
		elem->update();
	}

	return true;
}
